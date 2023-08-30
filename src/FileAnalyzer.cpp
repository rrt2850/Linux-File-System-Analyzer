/******************************************************************************
 * File: FileAnalyzer.cpp
 * Description: Analyzes individual files' attributes like size, type,
 *              and permissions.
 * Author: Robert Tetreault
 ******************************************************************************/

#include "FileAnalyzer.h"                   // header file for class definition
#include <iostream>                         // for printing to console
#include <sys/stat.h>                       // For the stat structure
#include <cerrno>                           // For errno
#include <cstring>                          // For strerror()

using std::string;
using std::cout;
using std::endl;
using std::cerr;

//
//  Constructors and Destructors
//

// Root directory constructor
FileAnalyzer::FileAnalyzer(const string& filePath) : path(filePath) {}
FileAnalyzer::FileAnalyzer(const string &filePath, const string &parent): path(filePath), parentPath(parent) {}
FileAnalyzer::~FileAnalyzer() {
    // Destructor
}

//
//  Getters
//

/******************************************************************************
 * getPath: Returns the path to the current file.
 * 
 * @return path: A string containing the path to the current file
 ******************************************************************************/
string FileAnalyzer::getPath() const {
    return path;
}

/******************************************************************************
 * getParentPath: Returns the path to the parent directory.
 * 
 * @return parentPath: A string containing the path to the parent directory
 ******************************************************************************/
string FileAnalyzer::getParentPath() const {
    return parentPath;
}

/******************************************************************************
 * getFileName: Returns the name of the current file.
 * 
 * @return fileName: A string containing the name of the current file
 ******************************************************************************/
string FileAnalyzer::getFileName() const {
    return fileName;
}

/******************************************************************************
 * getFileType: Returns the type of the current file.
 * 
 * @return fileType: A string containing the type of the current file
 ******************************************************************************/
string FileAnalyzer::getFileType() const {
    return fileType;
}

/******************************************************************************
 * getFilePermissions: Returns the permissions of the current file.
 * 
 * @return filePermissions: A string containing the permissions of the current file
 ******************************************************************************/
string FileAnalyzer::getFilePermissions() const {
    return filePermissions;
}

/******************************************************************************
 * getFileExtension: Returns the extension of the current file.
 * 
 * @return fileExtension: A string containing the extension of the current file
 ******************************************************************************/
string FileAnalyzer::getFileExtension() const {
    return fileExtension;
}

/******************************************************************************
 * getFileSize: Returns the size of the current file.
 * 
 * @return fileSize: A double containing the size of the current file
 ******************************************************************************/
double FileAnalyzer::getFileSize() const {
    return fileSize;
}


//
//  Public Methods
//

/******************************************************************************
 * analyzeFile: Analyzes the current file's attributes and stores them in
 *              the class's member variables.
 * 
 * @return void
 ******************************************************************************/
void FileAnalyzer::analyzeFile() {
    // Check if path was defined in constructor
    if(path.empty()){
        cerr << "Error: No file path specified." << endl;
        return;
    }

    struct stat fileInfo;
    
    // Check if file exists
    if(stat(path.c_str(), &fileInfo) != 0) {
        // Error occurred. Use errno and strerror to print error message.
        cerr << "Error accessing file " << path << ": " << strerror(errno) << endl;
        return;
    }

    // Set file size
    fileSize = static_cast<double>(fileInfo.st_size);

    // Analyzing file attributes
    findType(fileInfo);            // Set file type
    findPermissions(fileInfo);     // Set file permissions
    findName();                    // Set file name
    findExtension();               // Set file extension
}

/******************************************************************************
 * operator<<: Overloads the insertion operator to print the class's member
 *             variables to the console.
 * 
 * @param os: A reference to the output stream
 * @param obj: A reference to the FileAnalyzer object
 * @return os: A reference to the output stream
 ******************************************************************************/
std::ostream& operator<<(std::ostream& os, const FileAnalyzer& obj) {
    os << obj.fileName <<  ":" << endl;
    os << "\tPath: " << obj.path << endl;
    os << "\tParent Path: " << obj.parentPath << endl;
    os << "\tType: " << obj.fileType << endl;
    os << "\tExtension: " << obj.fileExtension << endl;
    os << "\tPermissions: " << obj.filePermissions << endl;
    os << "\tSize: " << obj.fileSize << " bytes" << endl;
    return os;
}

//
//  Private Methods
//

/******************************************************************************
 * findType: Determines the type of the current file.
 * 
 * @param fileInfo: A stat structure containing the current file's attributes
 * @return void
 * 
 * note:    This method is called by analyzeFile() and should not be called
 *          directly. There's no error checking because analyzeFile() already
 *          handles that.
 ******************************************************************************/
void FileAnalyzer::findType(struct stat fileInfo) {
    // Determine file type
    switch (fileInfo.st_mode & S_IFMT) {
    case S_IFREG:  fileType = "Regular File"; break;
    case S_IFDIR:  fileType = "Directory"; break;
    case S_IFLNK:  fileType = "Symbolic Link"; break;
    case S_IFBLK:  fileType = "Block Device"; break;
    case S_IFCHR:  fileType = "Character Device"; break;
    case S_IFIFO:  fileType = "FIFO"; break;
    case S_IFSOCK: fileType = "Socket"; break;
    default:       fileType = "Unknown"; break;
    }
}

/******************************************************************************
 * findPermissions: Determines the permissions of the current file.
 * 
 * @param fileInfo: A stat structure containing the current file's attributes
 * @return void
 * 
 * note:    This method is called by analyzeFile() and should not be called
 *          directly. There's no error checking because analyzeFile() already
 *          handles that.
 ******************************************************************************/
void FileAnalyzer::findPermissions(struct stat fileInfo) {
    // Extract file permissions, including special bits
    string perms = "";
    perms += (fileInfo.st_mode & S_IRUSR) ? "r" : "-";
    perms += (fileInfo.st_mode & S_IWUSR) ? "w" : "-";
    perms += (fileInfo.st_mode & S_IXUSR) ? "x" : "-";
    perms += (fileInfo.st_mode & S_IRGRP) ? "r" : "-";
    perms += (fileInfo.st_mode & S_IWGRP) ? "w" : "-";
    perms += (fileInfo.st_mode & S_IXGRP) ? "x" : "-";
    perms += (fileInfo.st_mode & S_IROTH) ? "r" : "-";
    perms += (fileInfo.st_mode & S_IWOTH) ? "w" : "-";
    perms += (fileInfo.st_mode & S_IXOTH) ? "x" : "-";

    filePermissions = perms;
}

/******************************************************************************
 * findName: Determines the name of the current file.
 * 
 * @return void
 * 
 * note:    This method is called by analyzeFile() and should not be called
 *          directly. There's no error checking because analyzeFile() already
 *          handles that.
 ******************************************************************************/
void FileAnalyzer::findName() {
    // Extract file name from path
    size_t found = path.find_last_of("/\\");        // Find last directory separator
    
    // If a directory separator was found, extract the file name and parent path
    if (found != string::npos) {
        fileName = path.substr(found + 1);          // Extract file name
        parentPath = path.substr(0, found);         // Extract parent path
    } else {
        fileName = path;  // If no directory separators were found, path is just the file name
        parentPath = "";
    }
}

/******************************************************************************
 * findExtension: Determines the extension of the current file.
 * 
 * @return void
 * 
 * note:    This method is called by analyzeFile() and should not be called
 *          directly. There's no error checking because analyzeFile() already
 *          handles that.
 ******************************************************************************/
void FileAnalyzer::findExtension() {
    // Extract file extension from file name
    size_t found = fileName.find_last_of(".");      // Find last period
    
    // If a period was found, extract the extension
    if (found != string::npos) {
        fileExtension = fileName.substr(found + 1); // Extract extension
    } else {
        fileExtension = "";                         // If no period was found, there is no extension
    }
}