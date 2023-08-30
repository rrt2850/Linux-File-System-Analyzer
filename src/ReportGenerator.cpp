/******************************************************************************
 * File: ReportGenerator.cpp
 * Description: Compiles analyzed data and produces something readable.
 * Author: Robert Tetreault
 ******************************************************************************/

#include "ReportGenerator.h"
#include "DirectoryReader.h"
#include "FileAnalyzer.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>

//
// Constructor and destructor
//

ReportGenerator::ReportGenerator(std::unordered_map<std::string, DirectoryReader> compDir)
    : completedDirectories(compDir) {}

ReportGenerator::~ReportGenerator() {
    // Nothing to do here
}

//
// Public methods
//

/******************************************************************************
 * sortDirectories: Sorts a vector of DirectoryReader objects by path.
 ******************************************************************************/
void ReportGenerator::sortDirectories(std::vector<DirectoryReader>& toSort) {
    std::sort(toSort.begin(), toSort.end(), 
        [](const DirectoryReader& a, const DirectoryReader& b) {
            return a.getPath() < b.getPath();
        });
}

/******************************************************************************
 * dumpInfoToFile:  Dumps all the information in the completedDirectories map
 *                  to a file with minimal formatting or organization.
 ******************************************************************************/
void ReportGenerator::dumpInfoToFile(std::string fileName) {
    std::ofstream outFile(fileName);    // Open the file for writing
    
    // Check if the file was opened or created successfully
    if (!outFile) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }

    // Write the values of the completedDirectories map to the file
    for (const auto& dir : completedDirectories) {
        outFile << "________________________________________________________________________________" << std::endl;
        outFile << dir.second.getPath() << std::endl;
        outFile << "Files: " << dir.second.getFiles().size() << std::endl;
        outFile << "Directories: " << dir.second.getDirectories().size() << std::endl;
        outFile << "Total size: " << dir.second.getTotalSize() << std::endl;
        outFile << "Average file size: " << dir.second.getAverageFileSize() << std::endl;
        outFile << "Average directory size: " << dir.second.getAverageDirectorySize() << std::endl;
        outFile << std::endl;
    }

    outFile.close();
}

/******************************************************************************
 * dumpPathsToFile: Dumps all the paths in the completedDirectories map to a
 *                  file with no formatting or organization.
 ******************************************************************************/
void ReportGenerator::dumpPathsToFile(std::string fileName) {
    std::ofstream outFile(fileName);    // Open the file for writing
    
    // Check if the file was opened or created successfully
    if (!outFile) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }

    // Write the values of the completedDirectories map to the file
    for (const auto& dir : completedDirectories) {
        outFile << dir.second.getPath() << std::endl;
    }

    outFile.close();
}

/******************************************************************************
 * dumpPathsToFileSorted: Dumps all the paths in the completedDirectories map
 *                        to a file with minimal formatting or organization.
 *                        The paths are sorted alphabetically.
 ******************************************************************************/
void ReportGenerator::dumpPathsToFileSorted(std::string fileName) {
    std::vector<DirectoryReader> sortedDirs;

    // Convert the completedDirectories map to a vector
    sortedDirs = Utilities::mapToVector(completedDirectories);

    // Check if the vector is empty
    if (sortedDirs.empty()) {
        std::cerr << "Error: No directories to sort" << std::endl;
        return;
    }

    std::ofstream outFile(fileName);    // Open the file for writing

    // Check if the file was opened or created successfully
    if (!outFile) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }

    // Sort the vector
    sortDirectories(sortedDirs);

    
    // Write the values of the completedDirectories map to the file
    for (const auto& dir : sortedDirs) {
        outFile << dir.getPath() << std::endl;
    }

    outFile.close();
}

/******************************************************************************
 * treeBuilder: Builds a tree of DirectoryReader objects from the
 *              completedDirectories map.
 * 
 * @param rootPath: The path of the root directory
 ******************************************************************************/
void ReportGenerator::treeBuilder(std::string rootPath) {

    // Check if the root directory exists in the completedDirectories map
    if (completedDirectories.find(rootPath) == completedDirectories.end()) {
        return;
    }

    //  Get the root DirectoryReader object
    DirectoryReader rootDir = completedDirectories[rootPath];

    // Check if the root directory is empty
    if (rootDir.getDirectories().empty() && rootDir.getFiles().empty()) {
        return;
    }

    // Print the tree
    printTree(rootDir);

    // Write the tree to a file
    std::ofstream outFile("output.txt");

    // Check if the file was opened or created successfully
    if (outFile.is_open()) {
        DirectoryReader rootDir = completedDirectories[rootPath];   //  Get the root DirectoryReader object
        printTreeToFile(rootDir, outFile);                          //  Print the tree to the file
        outFile.close();
    } else {
        std::cerr << "Unable to open file";
    }

}

/******************************************************************************
 * printTree: Recursive function to print directories and subdirectories with
 *            indentation and tree symbols.
 * 
 * @param dir: The DirectoryReader object to print
 * @param prefix: The prefix to use for indentation
 * @param isLast: Whether or not the current directory is the last in the list
 * @param isRoot: Whether or not the current directory is the root directory
 ******************************************************************************/
void ReportGenerator::printTree(const DirectoryReader& dir, std::string prefix, bool isLast, bool isRoot) {
    std::string newPrefix;

    // if the current directory is the root directory, print it and set isRoot to false since it's moving to the next level
    if (isRoot) {
        std::cout << dir.getPath() << std::endl;
        newPrefix = prefix;
        isRoot = false;
    } else {
        std::cout << prefix;                            // Print the prefix
        std::cout << (isLast ? "└─ " : "├─ ");          // Print the tree symbol depending on if it's the last directory
        std::cout << dir.getPath() << std::endl;        // Print the directory path

        newPrefix = prefix + (isLast ? "   " : "│  ");  // Update the prefix since we're going to print the subdirectories
    }

    // print all the subdirectories
    std::vector<std::string> subDirs = dir.getDirectories();
    for (size_t i = 0; i < subDirs.size(); ++i) {
        const std::string& subDirName = subDirs[i]; // Get the name of the subdirectory

        // Check if the subdirectory exists in the completedDirectories map and print it if it does
        if (completedDirectories.find(subDirName) != completedDirectories.end()) {
            printTree(completedDirectories[subDirName], newPrefix, i == subDirs.size() - 1 && dir.getFiles().empty(), false);
        }
    }

    // print all the files
    std::vector<FileAnalyzer> files = dir.getFiles();
    // Print the files in the same way as the subdirectories
    for (size_t i = 0; i < files.size(); ++i) {
        std::cout << newPrefix;
        std::cout << (i == files.size() - 1 ? "└─ " : "├─ ");
        std::cout << files[i].getFileName() << std::endl;
    }
}


/******************************************************************************
 * printTreeToFile: Recursive function to print directories and subdirectories
 *                  with indentation and tree symbols to a file.
 * 
 * @param dir: The DirectoryReader object to print
 * @param outFile: The file to write to
 * @param prefix: The prefix to use for indentation
 * @param isLast: Whether or not the current directory is the last in the list
 * @param isRoot: Whether or not the current directory is the root directory
 ******************************************************************************/
void ReportGenerator::printTreeToFile(const DirectoryReader& dir, std::ofstream& outFile, std::string prefix, bool isLast, bool isRoot) {
    std::string newPrefix;

    // if the current directory is the root directory, print it and set isRoot to false since it's moving to the next level
    if (isRoot) {
        outFile << dir.getPath() << std::endl;
        newPrefix = prefix;
        isRoot = false;
    } else {
        outFile << prefix;                      // Print the prefix
        outFile << (isLast ? "└─ " : "├─ ");    // Print the tree symbol depending on if it's the last directory
        outFile << dir.getPath() << std::endl;  // Print the directory path

        newPrefix = prefix + (isLast ? "   " : "│  ");  // Update the prefix since we're going to print the subdirectories
    }

    // print all the subdirectories
    std::vector<std::string> subDirs = dir.getDirectories();
    for (size_t i = 0; i < subDirs.size(); ++i) {
        const std::string& subDirName = subDirs[i]; // Get the name of the subdirectory

        // Check if the subdirectory exists in the completedDirectories map and print it if it does
        if (completedDirectories.find(subDirName) != completedDirectories.end()) {
            printTreeToFile(completedDirectories[subDirName], outFile, newPrefix, i == subDirs.size() - 1 && dir.getFiles().empty(), false);
        }
    }

    // print all the files
    std::vector<FileAnalyzer> files = dir.getFiles();

    // Print the files in the same way as the subdirectories
    for (size_t i = 0; i < files.size(); ++i) {
        outFile << newPrefix;
        outFile << (i == files.size() - 1 ? "└─ " : "├─ ");
        outFile << files[i].getFileName() << std::endl;
    }
}