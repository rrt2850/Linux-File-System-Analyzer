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
#include "DirectoryReader.h"
#include "ThreadPool.h"
#include "ReportGenerator.h"


int main() {
    std::string root = "/";

    // A list of directories that still need to be read
    std::vector<DirectoryReader> directoriesLeft = { DirectoryReader(root) };   
    
    // An unordered map of completed directories
    std::unordered_map<std::string, DirectoryReader> completedDirectories; 
    
    // An unordered map of deferred updates so 
    std::unordered_map<std::string, double> deferredUpdates;

    // Initialize the thread pool
    ThreadPool pool(100);

    // Mutex for synchronizing access to the directoriesLeft and completedDirectories vectors
    std::mutex dirMutex;

    DirectoryReader currentDir;

    while (true) {

        // Start critical section
        {
            std::unique_lock<std::mutex> lock(dirMutex);
            
            if (directoriesLeft.empty()) {
                if (pool.activeJobs == 0){
                    break;  // Exit the loop if no directories are left
                }
                else {
                    continue;  // Skip this iteration if there are no directories left but there are still active jobs
                }
            }
            
            currentDir = directoriesLeft.back();
            directoriesLeft.pop_back();
        }
        // End critical section

        pool.enqueue([currentDir, &dirMutex, &completedDirectories, &directoriesLeft, &deferredUpdates]() mutable {
            // Expensive operation (reading directory) is outside the lock
            currentDir.readDirectory();

            // Lock when dealing with shared resources
            {
                std::unique_lock<std::mutex> lock(dirMutex);

                // if the directory has been deferred and is now being read, update its total size
                if (deferredUpdates.find(currentDir.getPath()) != deferredUpdates.end()) {
                    // Add the deferred update to the directory's total size
                    currentDir.addToSubDirTotalSize(deferredUpdates[currentDir.getPath()]);

                    // Remove the deferred update
                    deferredUpdates.erase(currentDir.getPath());
                }

                // Add subdirectories to directoriesLeft
                if (!currentDir.getDirectories().empty()) {
                    for (const auto& dir : currentDir.getDirectories()) {
                        std::cout << "Adding directory: " << dir << std::endl;
                        directoriesLeft.push_back(DirectoryReader(dir, currentDir.getPath()));
                    }
                }

                // Check if it has a parent and update its totalsize
                std::string parentPath = currentDir.getParentPath();
                if (!parentPath.empty()) {

                    // If the parent directory has already been read, update its total size
                    if (completedDirectories.find(parentPath) != completedDirectories.end()) {
                        completedDirectories[parentPath].addToTotalSize(currentDir.getTotalSize());
                    } else {
                        // Add a deferred update
                        deferredUpdates[parentPath] += currentDir.getTotalSize();
                    }
                }

                // Add the current directory to the completedDirectories map
                completedDirectories[currentDir.getPath()] = currentDir;
            }
            // End of lock
        });
    }

    pool.waitForCompletion();

    ReportGenerator report(completedDirectories);
    report.treeBuilder(root);
    report.dumpInfoToFile("info.txt");

    return 0;
}