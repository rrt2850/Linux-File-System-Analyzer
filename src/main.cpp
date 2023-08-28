/******************************************************************************
 * File: Main.cpp
 * Description: The entry point of the application. It parses command-line
 *              arguments, manages the high-level flow of the program, and
 *              coordinates between different classes.
 * Author: Robert Tetreault
 ******************************************************************************/

#include <iostream>
#include "DirectoryReader.h"
#include "ThreadPool.h"

int main() {
    std::string root = "/";

    // A list of directories that still need to be read
    std::vector<DirectoryReader> directoriesLeft = { DirectoryReader(root) };   
    
    // A list of directories that have been read
    std::vector<DirectoryReader> completedDirectories;           

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

        // Use lambda to enqueue work to the thread pool
        pool.enqueue([currentDir, &dirMutex, &completedDirectories, &directoriesLeft]() mutable {
            currentDir.readDirectory();  // Read the directory

            {
                std::unique_lock<std::mutex> lock(dirMutex);
                
                if (!currentDir.getDirectories().empty()) {
                    for (const auto& dir : currentDir.getDirectories()) {
                        std::cout << "Adding directory: " << dir << std::endl;
                        directoriesLeft.push_back(DirectoryReader(dir, currentDir.getPath()));
                    }
                }
                completedDirectories.push_back(currentDir);  // Add to completed directories
            }
        });
    }

    pool.waitForCompletion();

    for (int i = 0; i < 10; i++) {
        DirectoryReader tempDir = completedDirectories[i];
        std::cout << "____________________________________________________________________________" << std::endl;
        std::cout << "|\t" << tempDir.getPath() << std::endl;
        std::cout << "|" << std::endl;

        for (FileAnalyzer file : tempDir.getFiles()) {
            std::cout << "- " << file << std::endl;
        }
    }

    return 0;
}