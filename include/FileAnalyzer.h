/******************************************************************************
 * File: FileAnalyzer.cpp
 * Description: Analyzes individual files' attributes like size, type,
 *              and permissions.
 * Author: Robert Tetreault
 ******************************************************************************/

#ifndef FILE_ANALYZER_H
#define FILE_ANALYZER_H

#include <vector>
#include <string>
#include <unordered_set>

class FileAnalyzer {
    public:
        
        // Constructors and Destructors

        FileAnalyzer(const std::string &filePath);
        FileAnalyzer(const std::string &filePath, const std::string &parent);
        ~FileAnalyzer();


        // Getters

        std::string getPath() const;
        std::string getParentPath() const;
        std::string getFileName() const;
        std::string getFileType() const;
        std::string getFilePermissions() const;
        double getFileSize() const;

        // Friend function to overload the insertion operator
        friend std::ostream& operator<<(std::ostream& os, const FileAnalyzer& obj);

        // Analyzes the current file
        void analyzeFile();                         

    private:

        // Variables

        std::string path;                       // The path to the current file to be analyzed
        std::string parentPath;                 // The path to the parent directory
        std::string fileName;                   // The name of the current file
        std::string fileType;                   // The type of the current file
        std::string filePermissions;            // The permissions of the current file
        double fileSize;                        // The size of the current file


        // Helper functions
        void findType(struct stat fileInfo);        // Determines the type of the current file
        void findPermissions(struct stat fileInfo); // Determines the permissions of the current file
        void findName();                            // Determines the name of the current file

};

#endif
