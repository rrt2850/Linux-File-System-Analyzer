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

/******************************************************************************
 * helper:  Prints a help message to the console explaining how to use the
 *          program.
 ******************************************************************************/
void helper(){
    std::cout << "Usage: ./main <root_directory> <output_file> [other_args...]" << std::endl
              << "Possible arguments:" << std::endl
              << "    -t:     Prints a tree of all the directories and files in the specified root" << std::endl
              << "    -ts:    Prints a tree of all the directories and files in the specified root to a file" << std::endl
              << "    -p:     Prints all the paths in the specified root directory and its subdirectories" << std::endl
              << "    -pa:    Prints all the paths in the specified root directory and its subdirectories sorted alphabetically" << std::endl
              << "    -ps:    Prints all the paths in the specified root directory and its subdirectories to a file" << std::endl
              << "    -psa:   Prints all the paths in the specified root directory and its subdirectories to a file sorted alphabetically" << std::endl
              << "    -i:     Prints all the information in the specified root directory and its subdirectories" << std::endl
              << "    -is:    Prints all the information in the specified root directory and its subdirectories to a file" << std::endl
              << "    -li  <numLevels (int)> : Print information about the first <numLevels> levels of directories" << std::endl
              << "    -lis <numLevels (int)> : Print information about the first <numLevels> levels of directories to a file" << std::endl
              << "    -lt  <numLevels (int)> : Print the tree of directories for the first <numLevels> levels" << std::endl
              << "    -lts <numLevels (int)> : Print the tree of directories to a file for the first <numLevels> levels" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc >= 2 && std::string(argv[1]) == "--help") {
        helper();
        return 0;
    }

    // Validate command-line arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <root_directory> <output_file> [other_args...]" << std::endl
                  << "For a list of arguments, run " << argv[0] << " --help" << std::endl;
        return 1;
    }

    // Check if no arguments were specified
    if (argc == 3) {
        std::cerr << "No arguments specified. For a list of arguments, run " << argv[0] << " --help" << std::endl;
        return 1;
    }

    // Initialize the root directory path
    std::string root = argv[1];
    std::string outputFile = argv[2];

    // Verify that the root directory is readable
    DirectoryReader tempDirChecker(root);
    if (!tempDirChecker.canReadDirectory()) {
        std::cerr << "\033[31mCannot read directory: " << root << "\033[0m" <<std::endl;
        return 1;
    }

    // Convert the remaining command-line arguments to a vector of strings
    std::vector<std::string> args(argv + 3, argv + argc);

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
                std::cerr << "\033[33mFailed to read directory: " << currentDir.getPath() << "\033[0m"<< std::endl;
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
    std::cout << "\033[32mWaiting for all thread pool jobs to complete...\033[0m" << std::endl;
    pool.waitForCompletion();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    std::cout << "\033[32mTotal time taken: " << duration << " seconds.\033[0m" << std::endl;

    if (exitCode.load() != 0) { // Check if any lambda function failed
        std::cerr << "\033[33mOne or more directory reads failed.\033[0m" << std::endl;
    }

    std::cout << "\033[32mGenerating report...\033[0m" << std::endl;

    // Generate a report based on the processed directories
    ReportGenerator report(completedDirectories);
    
    if (report.generateReport(outputFile, root, args) != 0) { // Check if the report generation failed
        std::cerr << "\033[31mFailed to generate report.\033[0m" << std::endl;
        return 1;
    }

    std::cout << "\033[32mReport successfully generated.\033[0m" << std::endl;

    return 0;  // Success
}
