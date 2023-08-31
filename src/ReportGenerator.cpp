/******************************************************************************
 * File: ReportGenerator.cpp
 * Description: Compiles analyzed data and produces something readable.
 * Author: Robert Tetreault
 ******************************************************************************/

#include "ReportGenerator.h"
#include "DirectoryReader.h"
#include "FileAnalyzer.h"
#include "Utilities.h"
#include <sstream>
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
 * dumpInfo:  Dumps all the information in the specified root directory
 * 
 * @param fileName: The name of the file to write to
 * @param root: The path of the root directory
 * @param mode: The mode to use
 * 
 * Possible modes:
 *      0: Print all information to the console
 *      1: Print all information to a file
 ******************************************************************************/
void ReportGenerator::dumpInfo(std::string fileName, std::string root, size_t mode) {
    std::ofstream outFile;

    // Open the file if mode is 1
    if (mode == 1) {
        outFile.open(fileName);
        if (!outFile) {
            std::cerr << "\033[31mError opening file for writing\033[0m" << std::endl;
            return;
        }
    }

    std::vector<DirectoryReader> dirs;
    collectSubdirectories(root, dirs);  // Collect all the subdirectories of the root directory

    // print the information for each directory to the console or to a file
    for (const auto& dir : dirs) {
        std::ostringstream oss;
        oss << "________________________________________________________________________________" << std::endl;
        oss << dir.getPath() << std::endl;
        oss << "Directories: " << dir.getDirectories().size() << std::endl;
        oss << "Total size: " << dir.getTotalSize() << std::endl;
        oss << "Average sub-directory size: " << dir.getAverageDirectorySize() << std::endl;
        oss << "Files: " << dir.getFiles().size() << std::endl;
        oss << "Average file size: " << dir.getAverageFileSize() << std::endl;
        oss << "Most common extension: " << dir.getTopFileExtension() << std::endl;
        oss << std::endl;

        if (mode == 0) {
            std::cout << oss.str();
        } else {
            outFile << oss.str();
        }
    }

    if (mode == 1) {
        outFile.close();
    }
}

/******************************************************************************
 * dumpInfoLevels:  Dumps all the information in the specified root directory
 *                  up to a specified level.
 * 
 * @param fileName: The name of the file to write to
 * @param root: The path of the root directory
 * @param mode: The mode to use
 * @param levels: The number of levels to consider
 * 
 * Possible modes:
 *      0: Print all information to the console
 *      1: Print all information to a file
 ******************************************************************************/
void ReportGenerator::dumpInfoLevels(std::string fileName, std::string root, size_t mode, size_t levels) {
    std::ofstream outFile;

    // Open the file if mode is 1
    if (mode == 1) {
        outFile.open(fileName);
        if (!outFile) {
            std::cerr << "\033[31mError opening file for writing\033[0m" << std::endl;
            return;
        }
    }

    std::vector<DirectoryReader> dirs;

    // Collect all the subdirectories of the root directory up to the given level
    collectSubdirectoriesLevels(root, dirs, levels);  

    // Print the information for each directory to the console or to a file
    for (const auto& dir : dirs) {
        std::ostringstream oss;
        oss << "________________________________________________________________________________" << std::endl;
        oss << dir.getPath() << std::endl;
        oss << "Directories: " << dir.getDirectories().size() << std::endl;
        oss << "Total size: " << dir.getTotalSize() << std::endl;
        oss << "Average sub-directory size: " << dir.getAverageDirectorySize() << std::endl;
        oss << "Files: " << dir.getFiles().size() << std::endl;
        oss << "Average file size: " << dir.getAverageFileSize() << std::endl;
        oss << "Most common extension: " << dir.getTopFileExtension() << std::endl;
        oss << std::endl;

        if (mode == 0) {
            std::cout << oss.str();
        } else {
            outFile << oss.str();
        }
    }

    if (mode == 1) {
        outFile.close();
    }
}


/******************************************************************************
 * collectSubdirectories: Recursively collects all the subdirectories of a
 *                        given directory.
 ******************************************************************************/
void ReportGenerator::collectSubdirectories(std::string root, std::vector<DirectoryReader>& dirs) {
    if (completedDirectories.find(root) == completedDirectories.end()) {
        return;
    }

    dirs.push_back(completedDirectories[root]);

    for (const auto& subDir : completedDirectories[root].getDirectories()) {
        collectSubdirectories(subDir, dirs);
    }
}

/******************************************************************************
 * collectSubdirectoriesLevels: Recursively collects all the subdirectories of
 *                              a given directory up to a specified level.
 * 
 * 
 ******************************************************************************/
void ReportGenerator::collectSubdirectoriesLevels(std::string root, std::vector<DirectoryReader>& dirs, size_t level) {
    if (completedDirectories.find(root) == completedDirectories.end()) {
        return;
    }

    dirs.push_back(completedDirectories[root]);

    if (level > 0) {
        for (const auto& subDir : completedDirectories[root].getDirectories()) {
            collectSubdirectoriesLevels(subDir, dirs, level - 1);
        }
    }
}

/******************************************************************************
 * dumpPaths: Dumps all the paths in the completedDirectories map to a file
 * 
 * @param fileName: The name of the file to write to
 * @param root: The path of the root directory
 * @param mode: The mode to use
 * 
 * Possible modes:
 *      0: Print all paths
 *      1: Print all paths sorted by size
 *      2: Dump all paths
 *      3: Dump all paths sorted by size
 ******************************************************************************/
void ReportGenerator::dumpPaths(std::string fileName, std::string root, size_t mode) {
    std::vector<DirectoryReader> dirs;
    collectSubdirectories(root, dirs);

    if (dirs.empty()) {
        std::cerr << "\033[31mError: No directories to sort\033[0m" << std::endl;
        return;
    }

    // Sorting logic here if mode is 1 or 3

    std::ofstream outFile;
    if (mode == 2 || mode == 3) {
        outFile.open(fileName);
        if (!outFile) {
            std::cerr << "\033[31mError opening file for writing\033[0m" << std::endl;
            return;
        }
    }

    for (const auto& dir : dirs) {
        if (mode < 2) {
            std::cout << dir.getPath() << std::endl;
        } else {
            outFile << dir.getPath() << std::endl;
        }
    }

    if (mode == 2 || mode == 3) {
        outFile.close();
    }
}

/******************************************************************************
 * treeBuilder: Builds a tree of all the directories in the specified root
 *              directory.
 * 
 * @param rootPath: The path of the root directory
 * @param mode: The mode to use
 * 
 * Possible modes:
 *      0: Print the tree to the console
 *      1: Print the tree to a file
 ******************************************************************************/
void ReportGenerator::treeBuilder(std::string fileName, std::string rootPath, size_t mode) {
    try {
    // Check if the root directory exists in the completedDirectories map
    if (completedDirectories.find(rootPath) == completedDirectories.end()) {
        std::cerr << "\033[31mError: Root directory does not exist\033[0m" << std::endl;
        return;
    }

    // Get the root DirectoryReader object
    DirectoryReader rootDir = completedDirectories[rootPath];

    // Check if the root directory is empty
    if (rootDir.getDirectories().empty() && rootDir.getFiles().empty()) {
        std::cerr << "\033[31mError: Root directory is empty\033[0m" << std::endl;
        return;
    }

    std::ostream* outStream;
    std::ofstream outFile;

    if (mode == 0) {
        outStream = &std::cout;
    } else {
        outFile.open(fileName);
        if (!outFile.is_open()) {
            std::cerr << "\033[31mUnable to open file\033[0m";
            return;
        }
        outStream = &outFile;
    }

    printTree(rootDir, "", true, true, *outStream);

     if (mode != 0) {
            outFile.close();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "\033[31mException in treeBuilder: " << e.what() << "\033[0m" << std::endl;
    }
}

/******************************************************************************
 * treeBuilderLevels: Builds a tree of all the directories in the specified root
 *                    directory up to a certain level.
 * 
 * @param rootPath: The path of the root directory
 * @param mode: The mode to use
 * @param levels: The number of levels deep to go
 * 
 * Possible modes:
 *      0: Print the tree to the console
 *      1: Print the tree to a file
 ******************************************************************************/
void ReportGenerator::treeBuilderLevels(std::string fileName, std::string rootPath, size_t mode, size_t levels) {
    if (completedDirectories.find(rootPath) == completedDirectories.end()) {
        return;
    }

    DirectoryReader rootDir = completedDirectories[rootPath];

    if (rootDir.getDirectories().empty() && rootDir.getFiles().empty()) {
        return;
    }

    std::ostream* outStream;
    std::ofstream outFile;

    if (mode == 0) {
        outStream = &std::cout;
    } else {
        outFile.open(fileName);
        if (!outFile.is_open()) {
            std::cerr << "\033[0mUnable to open file\033[0m";
            return;
        }
        outStream = &outFile;
    }

    printTreeLevels(rootDir, "", true, true, levels, *outStream);

    if (mode != 0) {
        outFile.close();
    }
}

/******************************************************************************
 * printTreeLevels: Prints a tree of directories and files up to a certain level
 *                  deep
 * 
 * @param dir: The DirectoryReader object to print
 * @param prefix: The prefix to use for each line
 * @param isLast: Whether or not the current directory is the last in the list
 * @param isRoot: Whether or not the current directory is the root
 * @param levels: The number of levels deep to go
 * @param outStream: The output stream to print to
 ******************************************************************************/
void ReportGenerator::printTreeLevels(const DirectoryReader& dir, std::string prefix, bool isLast,
                                      bool isRoot, size_t levels, std::ostream& outStream) {
    if (levels == 0) return;

    std::string newPrefix;
    if (isRoot) {
        outStream << dir.getPath() << std::endl;
        newPrefix = prefix;
        isRoot = false;
    } else {
        outStream << prefix << (isLast ? "└─ " : "├─ ") << dir.getPath() << std::endl;
        newPrefix = prefix + (isLast ? "   " : "│  ");
    }

    std::vector<std::string> subDirs = dir.getDirectories();
    for (size_t i = 0; i < subDirs.size(); ++i) {
        if (completedDirectories.find(subDirs[i]) != completedDirectories.end()) {
            printTreeLevels(completedDirectories[subDirs[i]], newPrefix, i == subDirs.size() - 1 && dir.getFiles().empty(), false, levels - 1, outStream);
        }
    }

    std::vector<FileAnalyzer> files = dir.getFiles();
    for (size_t i = 0; i < files.size(); ++i) {
        outStream << newPrefix << (i == files.size() - 1 ? "└─ " : "├─ ") << files[i].getFileName() << std::endl;
    }
}

/******************************************************************************
 * printTree: Prints a tree of all the directories and files in the specified
 *            root
 * 
 * @param dir: The DirectoryReader object to print
 * @param prefix: The prefix to use for each line
 * @param isLast: Whether or not the current directory is the last in the list
 * @param isRoot: Whether or not the current directory is the root
 * @param outStream: The output stream to print to
 ******************************************************************************/
void ReportGenerator::printTree(const DirectoryReader& dir, std::string prefix, bool isLast,
                                                        bool isRoot, std::ostream& outStream) {
    std::string newPrefix;
    try {
    if (isRoot) {
        outStream << dir.getPath() << std::endl;
        newPrefix = prefix;
        isRoot = false;
    } else {
        outStream << prefix << (isLast ? "└─ " : "├─ ") << dir.getPath() << std::endl;
        newPrefix = prefix + (isLast ? "   " : "│  ");
    }

    // Print the subdirectories
    std::vector<std::string> subDirs = dir.getDirectories();
    for (size_t i = 0; i < subDirs.size(); ++i) {
        if (completedDirectories.find(subDirs[i]) != completedDirectories.end()) {
            printTree(completedDirectories[subDirs[i]], newPrefix, i == subDirs.size() - 1 && dir.getFiles().empty(), false, outStream);
        }
    }
    
    // Print the files
    std::vector<FileAnalyzer> files = dir.getFiles();
    for (size_t i = 0; i < files.size(); ++i) {
        outStream << newPrefix << (i == files.size() - 1 ? "└─ " : "├─ ") << files[i].getFileName() << std::endl;
    }
        
    } catch (const std::exception& e) {
        std::cerr << "\033[31mException in printTree: " << e.what() << "\033[0m" << std::endl;
    }
}



/******************************************************************************
 * mapArgument: Maps a string argument to an Argument enum value for use in
 *              generateReport().
 ******************************************************************************/
Argument ReportGenerator::mapArgument(const std::string& arg) {
    if (arg == "-t") return TREE;
    if (arg == "-ts") return TREE_TO_FILE;
    if (arg == "-p") return PATHS;
    if (arg == "-pa") return SORTED_PATHS;
    if (arg == "-ps") return PATHS_TO_FILE;
    if (arg == "-psa") return SORTED_PATHS_TO_FILE;
    if (arg == "-i") return INFO;
    if (arg == "-is") return INFO_TO_FILE;
    if (arg == "-li") return LEVELS_INFO;
    if (arg == "-lis") return LEVELS_INFO_TO_FILE;
    if (arg == "-lt") return LEVELS_TREE;
    if (arg == "-lts") return LEVELS_TREE_TO_FILE;
    return UNKNOWN;
}

/******************************************************************************
 * generateReport: Generates a report based on the arguments passed in.
 * 
 * @param root: The path of the root directory
 * @param arguments: A vector of arguments passed in from the command line
 * 
 * Possible arguments:
 *      -t:     Prints a tree of all the directories and files in the specified root
 *      -ts:    Prints a tree of all the directories and files in the specified root to a file
 *      -p:     Prints all the paths in the specified root directory and its subdirectories
 *      -pa:    Prints all the paths in the specified root directory and its subdirectories sorted alphabetically
 *      -ps:    Prints all the paths in the specified root directory and its subdirectories to a file
 *      -psa:   Prints all the paths in the specified root directory and its subdirectories to a file sorted alphabetically
 *      -i:     Prints all the information in the specified root directory and its subdirectories
 *      -is:    Prints all the information in the specified root directory and its subdirectories to a file
 *      
 *      -li  <numLevels (int)> : Print information about the first <numLevels> levels of directories
 *      -lis <numLevels (int)> : Print information about the first <numLevels> levels of directories to a file
 *      -lt  <numLevels (int)> : Print the tree of directories for the first <numLevels> levels
 *      -lts <numLevels (int)> : Print the tree of directories to a file for the first <numLevels> levels
 ******************************************************************************/
int ReportGenerator::generateReport(std::string fileName, std::string root, std::vector<std::string> arguments) {
    int errorCode = 0; // 0 means no error
    std::ofstream reportFile;

    for (size_t i = 0; i < arguments.size(); ++i) {
        Argument arg;
        try {
            arg = mapArgument(arguments[i]);
        } catch (std::exception& e) {
            std::cerr << "\033[31mError mapping argument: " << e.what() << "\033[0m" << std::endl;
            errorCode = 1; // Update error code
            continue;
        }

        try {
            // Execute the appropriate action based on the argument
            switch (arg) {
                case TREE:
                    treeBuilder(fileName, root, 0);
                    break;
                case TREE_TO_FILE:
                    treeBuilder(fileName, root, 1);
                    break;
                case PATHS:
                    dumpPaths(fileName, root, 0);
                    break;
                case SORTED_PATHS:
                    dumpPaths(fileName, root, 1);
                    break;
                case PATHS_TO_FILE:
                    dumpPaths(fileName, root, 2);
                    break;
                case SORTED_PATHS_TO_FILE:
                    dumpPaths(fileName, root, 3);
                    break;
                case INFO:
                    dumpInfo(fileName, root, 0);
                    break;
                case INFO_TO_FILE:
                    dumpInfo(fileName, root, 1);
                    break;
                case LEVELS_INFO:
                    if (i + 1 < arguments.size()) {
                        size_t numLevels = std::stoi(arguments[i + 1]);
                        dumpInfoLevels(fileName, root, 0, numLevels);  
                        ++i; // Skip the level argument as it has been consumed
                    }
                    break;
                case LEVELS_INFO_TO_FILE:
                    if (i + 1 < arguments.size()) {
                        size_t numLevels = std::stoi(arguments[i + 1]);
                        dumpInfoLevels(fileName, root, 1, numLevels);  
                        ++i; 
                    }
                    break;
                case LEVELS_TREE:
                    if (i + 1 < arguments.size()) {
                        size_t numLevels = std::stoi(arguments[i + 1]);
                        treeBuilderLevels(fileName, root, 0, numLevels);  
                        ++i; 
                    }
                    break;
                case LEVELS_TREE_TO_FILE:
                    if (i + 1 < arguments.size()) {
                        size_t numLevels = std::stoi(arguments[i + 1]);
                        treeBuilderLevels(fileName, root, 1, numLevels); 
                        ++i; 
                    }
                    break;
                default:
                        std::cerr << "\033[31mUnknown argument: " << arguments[i] << "\033[0m" << std::endl;
                        errorCode = 2; // Update error code
                        break;
            }
        } catch (std::exception& e) {
            std::cerr << "\033[31mError executing action for argument: " << arguments[i] << ". Exception: " << e.what() << "\033[0m" <<std::endl;
            errorCode = 3; // Update error code
        }
    }

    return errorCode;
}