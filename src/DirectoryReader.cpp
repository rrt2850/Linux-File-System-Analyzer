/******************************************************************************
 * File: DirectoryReader.cpp
 * Description: Responsible for recursively reading directories and gathering
 *              file and sub-directory paths.
 * Author: Robert Tetreault
 ******************************************************************************/

// I used the path in the include because vscode freaks out if you don't.
#include "../include/DirectoryReader.h"     // header file for class definition
#include <iostream>                         // for printing to console

using std::string;
using std::vector;
using std::cout;
using std::endl;

DirectoryReader::DirectoryReader() {
    // Constructor 
}

DirectoryReader::~DirectoryReader() {
    // Destructor
}


/******************************************************************************
 * getListOfFiles: Returns a list of files in the given directory.
 * 
 * @param dirPath: The path to the directory to be searched.
 * @return fileList: A vector of strings, each string being the path to a file
 ******************************************************************************/
vector<string> DirectoryReader::getListOfFiles(const string &dirPath) {
    vector<string> fileList;
    
    cout << "getting files from: " << dirPath << endl;

    return fileList;
}


/******************************************************************************
 * getListOfDirectories: Returns a list of sub-directories in the given
 *                       directory.
 * 
 * @param dirPath: The path to the directory to be searched.
 * @return dirList: A vector of strings, each string being the path to a directory
 ******************************************************************************/
vector<string> DirectoryReader::getListOfDirectories(const string &dirPath) {
    vector<string> dirList;

    cout << "getting sub-directories from: " << dirPath << endl;

    return dirList;
}
