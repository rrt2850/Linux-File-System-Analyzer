/******************************************************************************
 * File: Main.cpp
 * Description: The entry point of the application. It parses command-line
 *              arguments, manages the high-level flow of the program, and
 *              coordinates between different classes.
 * Author: Robert Tetreault
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <chrono> 
#include "DirectoryReader.h"
#include "ThreadPool.h"
#include "ReportGenerator.h"

int main(int argc, char* argv[]) {
    // Validate command-line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <root_directory> [other_args...]" << std::endl
                  << "For a list of arguments, run " << argv[0] << " --help" << std::endl;
        return 1;
    }

    // Initialize the root directory path
    std::string root = argv[1];

    // Verify that the root directory is readable
    DirectoryReader tempDirChecker(root);
    if (!tempDirChecker.canReadDirectory()) {
        std::cerr << "Cannot read directory: " << root << std::endl;
        return 1;
    }

    // Convert the remaining command-line arguments to a vector of strings
    std::vector<std::string> args(argv + 2, argv + argc);

    // Initialize data structures for tracking directories
    std::vector<DirectoryReader> directoriesLeft = { DirectoryReader(root) };
    std::unordered_map<std::string, DirectoryReader> completedDirectories;
    std::unordered_map<std::string, double> deferredUpdates;
    std::atomic<int> exitCode = 0;  // To store the exit code in a thread-safe manner

    // Create a thread pool with 200 threads
    ThreadPool pool(200);

    // Mutex for thread-safe directory manipulation
    std::mutex dirMutex;

    DirectoryReader currentDir;

    auto start_time = std::chrono::high_resolution_clock::now();  // Time measurement

    // Main processing loop
    while (true) {
        {
            // Lock scope for thread-safe manipulation of shared resources
            std::unique_lock<std::mutex> lock(dirMutex);

            // Check if there are any directories left to process
            if (directoriesLeft.empty()) {
                if (pool.activeJobs == 0) {
                    break;
                } else {
                    continue;
                }
            }

            // Retrieve the next directory to process
            currentDir = directoriesLeft.back();
            directoriesLeft.pop_back();
        }

        // Enqueue a job in the thread pool to read and process the directory
        pool.enqueue([currentDir, &dirMutex, &completedDirectories, &directoriesLeft, &deferredUpdates, &exitCode]() mutable {
            // Attempt to read the directory; skip if failed
            if (!currentDir.readDirectory()) {
                std::cerr << "Failed to read directory: " << currentDir.getPath() << std::endl;
                exitCode = 1;  // Setting exit code to indicate failure
                return;  // Exit this lambda early if readDirectory() fails
            }

            // Lock scope for thread-safe manipulation of shared resources
            std::unique_lock<std::mutex> lock(dirMutex);

            // Handle any deferred updates for this directory
            if (deferredUpdates.find(currentDir.getPath()) != deferredUpdates.end()) {
                currentDir.addToSubDirTotalSize(deferredUpdates[currentDir.getPath()]);
                deferredUpdates.erase(currentDir.getPath());
            }

            // Enqueue any subdirectories for processing
            if (!currentDir.getDirectories().empty()) {
                for (const auto& dir : currentDir.getDirectories()) {
                    std::cout << "Adding directory: " << dir << std::endl;
                    directoriesLeft.push_back(DirectoryReader(dir, currentDir.getPath()));
                }
            }

            // Update the parent directory's total size, if applicable
            std::string parentPath = currentDir.getParentPath();
            if (!parentPath.empty()) {
                if (completedDirectories.find(parentPath) != completedDirectories.end()) {
                    completedDirectories[parentPath].addToTotalSize(currentDir.getTotalSize());
                } else {
                    deferredUpdates[parentPath] += currentDir.getTotalSize();
                }
            }

            // Mark this directory as completed
            completedDirectories[currentDir.getPath()] = currentDir;
        });
    }

    // Wait for all thread pool jobs to complete
    std::cout << "Waiting for all thread pool jobs to complete..." << std::endl;
    pool.waitForCompletion();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    std::cout << "Total time taken: " << duration << " seconds." << std::endl;

    if (exitCode.load() != 0) { // Check if any lambda function failed
        std::cerr << "One or more directory reads failed." << std::endl;
    }

    std::cout << "Generating report..." << std::endl;

    // Generate a report based on the processed directories
    ReportGenerator report(completedDirectories);
    
    if (report.generateReport("report.txt", root, args) != 0) { // Check if the report generation failed
        std::cerr << "Failed to generate report." << std::endl;
        return 1;
    }

    std::cout << "Report successfully generated." << std::endl;

    return 0;  // Success
}
