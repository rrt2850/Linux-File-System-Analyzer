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
    DirectoryReader reader("/mnt/c/");
    reader.readDirectory();

    std::vector<std::string> files = reader.getFiles();
    std::vector<std::string> directories = reader.getDirectories();

    std::cout << "Files:" << std::endl;
    for (const auto& file : files) {
        std::cout << file << std::endl;
    }

    std::cout << std::endl << "Directories:" << std::endl;
    for (const auto& directory : directories) {
        std::cout << directory << std::endl;
    }

    return 0;
}
