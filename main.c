/*
###########################################################
# Aaron Fortner                                           #
# COSC 4348-W01                                           #
# Systems Programming                                     #
# Assignment 4 - multi-process file copy                  #
###########################################################

 * Instructions
 * Write a C/C++ code to copy the content of one directory to another directory. Your program should
 * take as input two paths: a path to a source directory and a path to the destination directory. Your
 * code should satisfy the followings:
 * 1. You should use three processes: the parent and two children.
 * 2. Initially, the parent scans the source directory for files and subdirectories, and compute the total
 * size of the files in this directory. The parent then creates two children to copy the specified
 * directory. The parent should divide the tasks as follows:
 * • Find the median of the file sizes.
 * • One of the two children will copy all the files smaller the median and the other will copy
 * • all files larger than the median.
 * • The copied files will have the same permissions as the original files.
 * • The hierarchy of the source and the destination directory should be the same.
 * Do Not Forget
 * • Check for errors.
 * • Parent must wait for children.
 */

#include <dirent.h>
#include <wait.h>
#include "medianator.h"
#include "copier.h"
#include "tlpi_hdr.h"
#include "error_functions.h"

int main(int argc, char *argv[]) {

    pid_t high,low;

    // Check for correct number of arguments
    if (argc < 1 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s [dir...]\n", argv[0]);
    }

    char *source = argv[1];
    char *destination = argv[2];
    //check if destination directory exists, create if it doesn't
    if(opendir(destination) == NULL) {
        struct stat sourceStat;
        stat(source, &sourceStat);
        if(mkdir(destination, sourceStat.st_mode) == -1) {
            errExit("destination directory doesn't exist and unable to create it\n");
        }
    }

    //find median of all files in source directory and subdirectories
    median = medianFinder(source);
    printf("median file size: %ld\n", median);

    //fork two children to copy files
    if((low = fork()) == -1) {
        errExit("failed to fork low copy child process\n");
    }
    //only parent process will fork high copy child
    if(low != 0) {
        if((high = fork()) == -1) {
            errExit("failed to fork high copy child process\n");
        }
    }

    //low child copies files smaller than median
    if(low == 0 && high == 0) {
        printf("low child copying files less than or equal to median\n");
        if(directoryCopier(source, destination, median, 0) == -1) {
            errExit("couldn't copy directory low contents");
        }
        printf("low child finished copying files\n");
    //high child copies files larger than median
    } else if(low != 0 && high == 0) {
        printf("high child copying files greater than median\n");
        if(directoryCopier(source, destination, median, 1) == -1) {
            errExit("couldn't copy directory high contents");
        }
        printf("high child finished copying files\n");
    //parent waits for children
    } else {
        if(wait(&low) == -1) {
            errExit("wait failed on low child");
        }
        if(wait(&high) == -1) {
            errExit("wait failed on high child");
        }
        printf("parent process finished\n");
    }
    return 0;
}
