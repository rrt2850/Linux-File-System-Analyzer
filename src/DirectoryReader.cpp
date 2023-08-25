/******************************************************************************
 * File: DirectoryReader.cpp
 * Description: Responsible for recursively reading directories and gathering
 *              file and sub-directory paths.
 * Author: Robert Tetreault
 ******************************************************************************/

#include "DirectoryReader.h"     // header file for class definition
#include <iostream>                         // for printing to console
#include <dirent.h>                         // For directory functions
#include <sys/types.h>                      // For data types used by dirent.h
#include <sys/stat.h>                       // For the stat structure
#include <cerrno>                           // For errno

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cerr;

// Constructor that sets the specified path as the path variable
DirectoryReader::DirectoryReader(const string& dirPath) : path(dirPath) {}

DirectoryReader::DirectoryReader() {
    // Constructor 
}

DirectoryReader::~DirectoryReader() {
    // Destructor
}


/******************************************************************************
 * getFiles: Returns a list of files if the directory is already specified.
 * 
 * @return files: A vector of strings, each string being the path to a file
 ******************************************************************************/
vector<string> DirectoryReader::getFiles() const {
    return files;
}

/******************************************************************************
 * getDirectories: Returns a list of sub-directories in the given
 *                       directory.
 * 
 * @return directories: A vector of strings, each string being the path to a
 *                      directory
 ******************************************************************************/
vector<string> DirectoryReader::getDirectories() const {
    return directories;
}



/******************************************************************************
 * readDirectory: Reads the directory specified in the constructor without
 *                   recursion.
 ******************************************************************************/
void DirectoryReader::readDirectory() {
    DIR* dir;                   // Pointer to a directory stream
    struct dirent* entry;       // Pointer to a directory entry
    struct stat entInfo;        // Information about the directory entry

    // Reset the errno variable
    errno = 0;

    // Open the directory as a stream
    dir = opendir(path.c_str());

    if (dir != NULL) {
        // Read files and directories within the current directory
        while ((entry = readdir(dir)) != NULL) {

            // Get the full path of the entry
            string fullpath = path + "/" + entry->d_name;

            // Check if the entry is a directory
            if (stat(fullpath.c_str(), &entInfo) == 0 && S_ISDIR(entInfo.st_mode)) {
                // The entry is a directory

                // Skip '.' and '..' to avoid infinite loops
                if (string(entry->d_name) != "." && string(entry->d_name) != "..") {
                    // Add the directory name to the list of directories
                    directories.push_back(entry->d_name);
                }
            } else {
                // The entry is a file

                // Add the file name to the list of files
                files.push_back(entry->d_name);
            }
        }

        // Check if readdir() stopped due to an error
        if (errno != 0) {
            cerr << "Error reading directory: " << path << endl;
        }

        closedir(dir);
    } else {
        cerr << "Could not open directory: " << path << endl;
    }
}

