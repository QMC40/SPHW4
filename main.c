/*
###########################################################
# Aaron Fortner                                           #
# COSC 4348-W01                                           #
# Systems Programming                                     #
# Assignment 4 - threaded file copy                       #
###########################################################

 * Instructions
 * Write a C/C++ code to copy the content of one directory to another directory. Your program should
 * take as input two paths: a path to a source directory and a path to the destination directory. Your
 * code should satisfy the followings:
 * 1. You should use three processes: the parent and two children.
 * 2. Initially, the parent scans the source directory for files and sub-directories, and compute the total
 * size of the files in this directory. The parent then creates two children to copy the specified
 * directory. The parent should divide the tasks as follows:
 * • Find the median of the file sizes.
 * • One of the two children will copy all the files smaller the median and the other will copy
 * all files larger than the median.
 * • The copied files will have the same permissions as the original files.
 * • The hierarchy of the source and the destination directory should be the same.
 * Do Not Forget
 * • Check for errors.
 * • Parent must wait for children.
 */


/*
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[]) {

    int runs = 5;
    char *source[] = {"jobsource.txt"};
    char *destination[] = {"jobcopy.txt"};
    if (argc == 4) {
        *source = (char *) argv[1];
        *destination = (char *) argv[2];
        runs = atoi(argv[3]);
    }
    for (int i = 1; i < 4097; i = i * 2) {
        unsigned int catch = copyfile(source, destination, runs, i);
        printf("average time to copy over %d runs with buffer size %d: %u seconds %u milliseconds\n", runs, i,
               catch / 1000, catch % 1000);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

    struct dirent *dirSource;
    struct dirent *dirDest;

    DIR *source;
    DIR *destination;

    if (argc == 3) {
        if ((source = opendir(argv[1])) == NULL) {
            perror("Failed to open source directory");
            return 1;
        }
        if ((destination = opendir(argv[2])) == NULL) {
            perror("Failed to open destination directory");
            return 1;
        }
    }else {
        fprintf(stderr, "Usage: %s source: directory_name destination: directory_name\n", argv[0]);
        return 1;
    }

    dirDest = readdir(destination);
    while ((dirSource = readdir(source)) != NULL) {
        printf("%s\n", dirSource->d_name);
        char *destPath = malloc(255);//destination paths for I/O ops
        strcpy(destPath,dirDest->d_name);
        strcat(destPath,"/");
        strcpy(destPath, dirSource->d_name);
        mkdir(destPath,0777);
//        printf("destPath: %s\n",destPath);
//        mkdir(destPath,0777);
    }

    while ((closedir(source) == -1) && (errno == EINTR));
    return 0;
}

*/

#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <sys/fcntl.h>
#include "medianator.h"
#include "tlpi_hdr.h"
#include "error_functions.h"

int main(int argc, char *argv[]) {

    int listFd;
    long median;
    bool byDir = false; //true if to copy using median of each directory, false if just using median of all files

    if (argc < 1 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s [dir...]\n", argv[0]);
    }

    char *source = argv[1];
    char *destination = argv[2];

    median = medianFinder(source);
    printf("median: %ld\n", median);
    return 0;
}
