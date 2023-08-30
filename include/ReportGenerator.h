/******************************************************************************
 * File: ReportGenerator.h
 * Description: Compiles analyzed data and produces something readable.
 * Author: Robert Tetreault
 ******************************************************************************/

#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include <vector>
#include <unordered_map>
#include "DirectoryReader.h"

class ReportGenerator {
    public:

        //
        //  Constructors and destructor
        //
        ReportGenerator(std::unordered_map<std::string, DirectoryReader> compDir);
        ~ReportGenerator();

        
        void generateReport();

        //   
        void treeBuilder(std::string root);

        //  Sorts a vector of DirectoryReader objects by path.
        void sortDirectories(std::vector<DirectoryReader>& toSort);

        //  Dumps all the information in the completedDirectories map to an unorganized text file.
        void dumpInfoToFile(std::string fileName);

        //  Dumps all the paths in the completedDirectories map to a file
        void dumpPathsToFile(std::string fileName);

        //  dumpPathsToFileSorted: Dumps all the paths in the completedDirectories map to a file sorted alphabetically
        void dumpPathsToFileSorted(std::string fileName);
        

    private:
        //  A map of all the directories that have been read
        std::unordered_map<std::string, DirectoryReader> completedDirectories;
        
        //  recursively prints the directories as a tree
        void printTree(const DirectoryReader& dir, std::string prefix = "", bool isLast = true, bool isRoot = true);
        
        //  recursively prints the directories as a tree to a file
        void printTreeToFile(const DirectoryReader& dir, std::ofstream& outFile, std::string prefix = "",
                             bool isLast = true, bool isRoot = true);
};

#endif