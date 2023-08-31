# Linux File System Analyzer

### Description
-   This is a c++ program that analyzes the file system of a linux machine and outputs the results to a file.

### Highlights of this project
-   Uses an implementation of threadpool to speed up the analysis of the file system.
-   Uses a complicated tree structure to make the directories and files easy to read.
-   Uses objects for everything to make the code modular and easy to read.
-   Uses a lot of different options to customize the output of the program.
-   Uses escape sequences to make the output of the program colorful and easy to read.
-   Has decent error handling to make sure the program doesn't crash.
-   Has a makefile to make compiling and running the program easy.
-   Has a lot of comments and documentation to make the code easy to understand from a glance.

### Usage
-   Run the program by first compiling it with the command `make` and then running it with the command `./LFSA <root> <outputFile> <options>`
    -   `<root>` is the root directory to start the analysis from
    -   `<outputFile>` is the file to output the results to
    -   `<options>` is a list of options to customize the output of the program
        -   See the section below for a list of all the options
-   There are a ton of different options that can be used to customize the output of the program.
    - -t: Prints a tree of all the directories and files in the specified root
    - -ts: Prints a tree of all the directories and files in the specified root to a file
    - -p: Prints all the paths in the specified root directory and its subdirectories
    - -pa: Prints all the paths in the specified root directory and its subdirectories sorted alphabetically
    - -ps: Prints all the paths in the specified root directory and its subdirectories to a file
    - -psa: Prints all the paths in the specified root directory and its subdirectories to a file sorted alphabetically
    - -i: Prints all the information in the specified root directory and its subdirectories
    - -is: Prints all the information in the specified root directory and its subdirectories to a file
    - -li  <numLevels (int)> : Print information about the first <numLevels> levels of directories
    - -lis <numLevels (int)> : Print information about the first <numLevels> levels of directories to a file
    - -lt  <numLevels (int)> : Print the tree of directories for the first <numLevels> levels
    - -lts <numLevels (int)> : Print the tree of directories to a file for the first <numLevels> levels