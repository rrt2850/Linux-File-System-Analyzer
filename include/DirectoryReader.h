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
        DirectoryReader();
        ~DirectoryReader();

        // Retrieves a list of files in the given directory.
        std::vector<std::string> getListOfFiles(const std::string &dirPath);

        // Retrieves a list of sub-directories in the given directory.
        std::vector<std::string> getListOfDirectories(const std::string &dirPath);

};

#endif
