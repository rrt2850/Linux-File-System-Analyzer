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
        ReportGenerator(std::unordered_map<std::string, DirectoryReader> compDir);
        ~ReportGenerator();

        void generateReport();
        void treeBuilder(std::string root);
        void sortDirectories(std::vector<DirectoryReader>& toSort);
        void dumpInfoToFile(std::string fileName);
        void dumpPathsToFile(std::string fileName);
        void dumpPathsToFileSorted(std::string fileName);
        

    private:
        std::unordered_map<std::string, DirectoryReader> completedDirectories;
        void printTree(const DirectoryReader& dir, std::string prefix = "", bool isLast = true, bool isRoot = true);
        void printTreeToFile(const DirectoryReader& dir, std::ofstream& outFile, std::string prefix = "",
                             bool isLast = true, bool isRoot = true);
};

#endif