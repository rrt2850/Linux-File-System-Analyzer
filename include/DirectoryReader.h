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

class DirectoryReader {
    public:
        DirectoryReader(const std::string &dirPath);
        DirectoryReader();
        ~DirectoryReader();

        // Reads the directory specified in the constructor.
        void readDirectory();

        // Retrieves a list of files in the directory specified in the constructor.
        std::vector<std::string> getFiles() const;

        // Retrieves a list of sub-directories in the directory specified in the constructor.
        std::vector<std::string> getDirectories() const;

    private:
        std::string path;                       // The path to the current directory to be searched
        std::vector<std::string> files;         // A list of files in the current directory
        std::vector<std::string> directories;   // A list of sub-directories in the current directory

};

#endif
