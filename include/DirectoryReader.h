/******************************************************************************
 * File: DirectoryReader.h
 * Description: Responsible for recursively reading directories and gathering
 *              file and sub-directory paths.
 * Author: Robert Tetreault
 ******************************************************************************/

#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H

#include <vector>
#include <string>
#include <unordered_set>

class DirectoryReader {
    public:
        // A list of directories to skip
        const std::vector<std::string> SKIP_DIRECTORIES = {"/mnt/"};

        DirectoryReader(const std::string &dirPath);
        ~DirectoryReader();

        // Reads the directory specified in the constructor.
        void readDirectory();

        // Retrieves a list of files in the directory specified in the constructor.
        std::vector<std::string> getFiles() const;

        // Retrieves a list of sub-directories in the directory specified in the constructor.
        std::vector<std::string> getDirectories() const;

        // Retrieves the path of the directory specified in the constructor.
        std::string getPath() const;

        // Retrieves the total size of all files in the directory specified in the constructor.
        double getTotalSize() const;

        // Retrieves the number of files in the directory specified in the constructor.
        int getNumFiles() const;

    private:
        std::string path;                       // The path to the current directory to be searched
        std::vector<std::string> files;         // A list of files in the current directory
        std::vector<std::string> directories;   // A list of sub-directories in the current directory
        double totalSize;                       // The total size of all files in the current directory
        int numFiles;                           // The number of files in the current directory
};

#endif
