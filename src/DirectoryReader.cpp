/******************************************************************************
 * File: DirectoryReader.cpp
 * Description: Responsible for recursively reading directories and gathering
 *              file and sub-directory paths.
 * Author: Robert Tetreault
 ******************************************************************************/

#include "DirectoryReader.h"                // header file for class definition
#include <iostream>                         // for printing to console
#include <dirent.h>                         // For directory functions
#include <sys/types.h>                      // For data types used by dirent.h
#include <sys/stat.h>                       // For the stat structure
#include <cerrno>                           // For errno
#include <cstring>                          // For strerror()
#include <unordered_set>                    // For directories to skip

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cerr;

//
//  Constructors and Destructors
//

// Constructor that sets the specified path as the path variable
DirectoryReader::DirectoryReader(const string& dirPath) : path(dirPath) {}

DirectoryReader::~DirectoryReader() {
    // Destructor
}

//
//  Getters
//

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
 * getTotalSize: Returns the total size of all files in the directory.
 * 
 * @return totalSize: The total size of all files in the directory
 ******************************************************************************/
double DirectoryReader::getTotalSize() const {
    return totalSize;
}

/******************************************************************************
 * getNumFiles: Returns the number of files in the directory.
 * 
 * @return numFiles: The number of files in the directory
 ******************************************************************************/
int DirectoryReader::getNumFiles() const {
    return numFiles;
}


/******************************************************************************
 * getPath: Returns the path of the directory.
 * 
 * @return path: The path of the directory
 ******************************************************************************/
string DirectoryReader::getPath() const {
    return path;
}


//
//  Public Methods
//

/******************************************************************************
 * readDirectory:   Reads the directory specified in the constructor and stores
 *                  the files and sub-directories in the files and directories
 ******************************************************************************/
void DirectoryReader::readDirectory() {
    DIR* dir;                   // Pointer to a directory stream
    struct dirent* entry;       // Pointer to a directory entry
    struct stat entInfo;        // Information about the directory entry
    string fullpath;            // A string to hold the full path of the entry

    // Reset the errno variable
    errno = 0;

    // Open the directory as a stream
    dir = opendir(path.c_str());
    if (dir == NULL) {
        cerr << "Error opening directory: " << path << ". Error: " << strerror(errno) << endl;
        return;  // or handle this appropriately
    }

    
    // Read files and directories within the current directory
    while ((entry = readdir(dir)) != NULL) {
        
        // Get the full path of the entry
        if(path != "/"){
            fullpath = path + "/" + entry->d_name;
        }
        else{
            fullpath = path + entry->d_name;
        }
        
        if (lstat(fullpath.c_str(), &entInfo) == -1) {
            cerr << "Error stat-ing path: " << fullpath << ". Error: " << strerror(errno) << endl;
            continue;  // move on to the next directory entry
        }
        
        if (S_ISLNK(entInfo.st_mode)) {
            continue;  // skip symbolic links
        }

        // Check if the entry is a directory
        if (S_ISDIR(entInfo.st_mode)) {
            // The entry is a directory

            // Only do all this work if there are directories to skip
            if(SKIP_DIRECTORIES.size() > 0){
                bool shouldSkip = false;
                for (auto skipDir : SKIP_DIRECTORIES) {
                    // Check if the directory should be skipped
                    if (fullpath.find(skipDir) != string::npos) {
                        shouldSkip = true;
                        break;  // exit the for loop
                    }
                }
                if (shouldSkip) {
                    continue;  // continue to the next iteration of the while loop
                }
            }

            // Skip '.' and '..' to avoid infinite loops
            if (string(entry->d_name) != "." && string(entry->d_name) != "..") {
                // Add the directory name to the list of directories
                directories.push_back(fullpath);
            }

        } else {
            // The entry is a file

            // Add the file name to the list of files
            files.push_back(fullpath);
        }
    }

    // Check if readdir() stopped due to an error
    if (errno != 0) {
        cerr << "Error reading directory: " << path << ". Error: " << strerror(errno) << endl;
    }

    closedir(dir);
}
