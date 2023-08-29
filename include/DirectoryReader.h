/******************************************************************************
 * File: DirectoryReader.h
 * Description: Responsible for reading directories and gathering file and
 *              sub-directory paths.
 * Author: Robert Tetreault
 ******************************************************************************/

#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H

#include "FileAnalyzer.h"
#include <vector>
#include <string>
#include <unordered_set>


class DirectoryReader {
    public:
        // A list of directories to skip
        const std::vector<std::string> SKIP_DIRECTORIES = {"/mnt/"};

        DirectoryReader();
        DirectoryReader(const std::string &dirPath);
        DirectoryReader(const std::string &dirPath, const std::string &parent);
        ~DirectoryReader();

        // Reads the directory specified in the constructor.
        void readDirectory();

        // Retrieves a list of files in the directory specified in the constructor.
        std::vector<FileAnalyzer> getFiles() const;

        // Retrieves a list of sub-directories in the directory specified in the constructor.
        std::vector<std::string> getDirectories() const;

        // Retrieves the path of the directory specified in the constructor.
        std::string getPath() const;

        // Retrieves the path of the parent directory.
        std::string getParentPath() const;

        // Retrieves the average size of all files in the directory specified in the constructor.
        double getAverageFileSize() const;

        // Retrieves the average size of all sub-directories in the directory specified in the constructor.
        double getAverageDirectorySize() const;

        // Retrieves the total size of all files in the directory specified in the constructor.
        double getTotalSize() const;

        // Retrieves the number of files in the directory specified in the constructor.
        int getNumFiles() const;

        DirectoryReader& operator=(const DirectoryReader& other);

    private:
        std::string path;                       // The path to the current directory to be searched
        std::string parentPath;                 // The path to the parent directory
        std::vector<FileAnalyzer> files;        // A list of files in the current directory
        std::vector<std::string> directories;   // A list of sub-directories in the current directory
        double totalSize;                       // The size of all files and sub-directories in the current directory
        double fileTotalSize;                   // The size of all files in the current directory
        double dirTotalSize;                    // The size of all sub-directories in the current directory
        int numFiles;                           // The number of files in the current directory
};

#endif
