/******************************************************************************
 * File: Main.cpp
 * Description: The entry point of the application. It parses command-line
 *              arguments, manages the high-level flow of the program, and
 *              coordinates between different classes.
 * Author: Robert Tetreault
 ******************************************************************************/

#include <iostream>
#include "DirectoryReader.h"

int main() {
    std::string root = "/";
    std::vector <DirectoryReader> directoriesLeft = { DirectoryReader(root) };
    std::vector<DirectoryReader> completedDirectories;
    
    while (!directoriesLeft.empty()) {
        DirectoryReader currentDir = directoriesLeft.back();
        directoriesLeft.pop_back();

        currentDir.readDirectory();                     // Read the directory
        completedDirectories.push_back(currentDir);     // Add the directory to the list of completed directories

        // Initialize the list of directories to explore and add them to the list
        if (currentDir.getDirectories().size() > 0) {
            for (auto dir : currentDir.getDirectories()) {
                std::cout << "Adding directory: " << dir << std::endl;
                directoriesLeft.push_back(DirectoryReader(dir));
            }
            std::cout << "___________________________________________________________________________" << std::endl;
        }
    }

    std::cout << "Completed directories: " << std::endl;
    for (DirectoryReader dir : completedDirectories) {
        std::cout << dir.getPath() << std::endl;
    }

    

    return 0;
}