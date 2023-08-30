/******************************************************************************
 * File: ReportGenerator.h
 * Description: Compiles analyzed data and produces something readable.
 * Author: Robert Tetreault
 ******************************************************************************/

#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <vector>
#include <unordered_map>
#include <iostream>
#include "DirectoryReader.h"


//  The different types of arguments that can be passed to the program
enum Argument {
    TREE,
    TREE_TO_FILE,
    PATHS,
    SORTED_PATHS,
    PATHS_TO_FILE,
    SORTED_PATHS_TO_FILE,
    INFO,
    INFO_TO_FILE,
    LEVELS_INFO,
    LEVELS_INFO_TO_FILE,
    LEVELS_TREE,
    LEVELS_TREE_TO_FILE,
    UNKNOWN
};

class ReportGenerator {
    public:

        //
        //  Constructors and destructor
        //
        ReportGenerator(std::unordered_map<std::string, DirectoryReader> compDir);
        ~ReportGenerator();

        
        int generateReport(std::string fileName = "report.txt", std::string root = "/", std::vector<std::string> arguments = {});
        

    private:
        //  A map of all the directories that have been read
        std::unordered_map<std::string, DirectoryReader> completedDirectories;

        //  Sorts a vector of DirectoryReader objects by path.
        void sortDirectories(std::vector<DirectoryReader>& toSort);

        //  Recursively collects all the subdirectories of a given directory
        void collectSubdirectories(std::string root, std::vector<DirectoryReader>& dirs);

        //  Recursively collects all the subdirectories of a given directory up to a given level
        void collectSubdirectoriesLevels(std::string root, std::vector<DirectoryReader>& dirs, size_t level);

        //  recursively prints the directories as a tree
        void printTree(const DirectoryReader& dir, std::string prefix = "", bool isLast = true,
                                                bool isRoot = true, std::ostream& outStream = std::cout);


        //  recursively prints the directories as a tree up to a given level
        void printTreeLevels(const DirectoryReader& dir, std::string prefix, bool isLast,
                                      bool isRoot, size_t levels, std::ostream& outStream);

        //  Dumps all the paths in the completedDirectories map to a file
        //  mode 0 = print all paths
        //  mode 1 = print all paths sorted by size
        //  mode 2 = dump all paths
        //  mode 3 = dump all paths sorted by size
        void dumpPaths(std::string fileName, std::string root, size_t mode=0);

        //  Prints the directories as a tree
        //  mode 0 = print to console
        //  mode 1 = print to file
        void treeBuilder(std::string fileName= "treeLevels.txt", std::string rootPath = "", size_t mode=0);

        //  Prints the directories as a tree up to a given level
        //  mode 0 = print to console
        //  mode 1 = print to file
        void treeBuilderLevels(std::string fileName= "treeLevels.txt", std::string root="/" , size_t mode = 0, size_t level = 0);

        //  dumps all the information about the subdirectories of a given directory
        //  mode 0 = print to console
        //  mode 1 = print to file
        void dumpInfo(std::string fileName, std::string root, size_t mode);
        
        //  dumps all the information about the subdirectories of a given directory up to a given level
        //  mode 0 = print to console
        //  mode 1 = print to file
        void dumpInfoLevels(std::string fileName, std::string root, size_t mode, size_t level);
        
        //  maps a string to an Argument enum
        Argument mapArgument(const std::string& arg);
};

#endif