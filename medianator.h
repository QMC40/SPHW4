/*
###########################################################
# Aaron Fortner                                           #
# COSC 4348-W01                                           #
# Systems Programming                                     #
# Assignment 4 - multi-process file copy                  #
###########################################################
*/

#ifndef VERS1_MEDIANATOR_H
#define VERS1_MEDIANATOR_H

#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <sys/fcntl.h>
#include "tlpi_hdr.h"
#include "error_functions.h"

long totalSize = 0;
int fileCount = 0;
long *fileSizes = NULL;
int i = 0;

//compare function for qsort
int compare(const void *a, const void *b) {
    return (*(long *) a > *(long *) b) ? 1 : -1;
}

//print array contents to stdout     TESTING CODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void printArray(long array[], int size) {
    for (i = 0; i < size; i++) {
        printf("%ld ", array[i]);
    }
    printf("\n");
}

//print directory contents to stdout    TESTING CODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int directoryPrint(const char *pathname, const struct stat *sbuf, int type,
                   struct FTW *ftwb) {
        if(S_ISDIR(sbuf->st_mode)) {
            return 0;
        }
    printf(" %*s", 4 * ftwb->level, ""); /* Indent suitably */
    printf("%s\n", &pathname[ftwb->base]); /* Print basename */

    return 0; /* Tell nftw() to continue */
}

//take the list of file sizes, sort and select and return median file size
long finder() {
    qsort(fileSizes, fileCount, sizeof(long), compare);
//    printArray(fileSizes, fileCount);
    long median = 0;
    //empty directory kickback
    if (fileCount <= 0) { return 0; }
    //find middle element (int div to trunc for odd count)
    int med = (fileCount - 1) / 2;
    if (fileCount % 2 == 0) {
        median = (fileSizes[med] + fileSizes[med + 1]) / 2;
    } else {
        median = fileSizes[med];
    }
    //test print of result, using float div for element to test even / odd list handling
//    printf("median is: %ld @ %.1f\n", median,(fileCount - 1) / 2.0);    // TESTING CODE
    return median;
}

//transit the directory tree and count files
int counter(const char *dirpath, const struct stat *sbuf, int type,
            struct FTW *ftwb) {
    if (!S_ISDIR(sbuf->st_mode)) {
        fileCount++;
        totalSize += sbuf->st_size;
    }
    return 0;  /* Tell nftw() to continue */
}

//fill the created dynamic array with file sizes
int stacker(const char *dirpath, const struct stat *sbuf, int type,
            struct FTW *ftwb) {
    if (!S_ISDIR(sbuf->st_mode)) {
        fileSizes[i] = sbuf->st_size;
        i++;
    }
    return 0; /* Tell nftw() to continue */
}

//find the median file size in a directory tree
long medianFinder(const char *dirpath) {

    fileCount = 0, totalSize = 0;
    //prints listing of directory supplied w/ *dirpath  //TESTING CODE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//    if(nftw(dirpath, directoryPrint, 10, 0) != 0) {
//        errExit("unable to complete walking file tree for directory");
//    }

    //iteration counter set to 0 and count files in directory tree
    i = 0;
    if(nftw(dirpath, counter, 10, 0) != 0) {
        errExit("unable to complete walking file tree for counting files");
    }

    printf("file count: %d - total size: %ld ", fileCount, totalSize);  //TESTING CODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //create a dynamic array large enough to hold all file sizes, exit if unable to create
    fileSizes = (long *) calloc(fileCount, sizeof(long));
    if (fileSizes == NULL) {
        errExit("unable to create file listing");
    }

    //iteration counter set to 0 and add file sizes from directory tree
    i = 0;
   if(nftw(dirpath, stacker, 10, 0) != 0) {
        errExit("unable to complete walking file tree for adding file sizes");
    }

    //get median file size
    long median = finder();

    //release dynamic array used to find median
    free(fileSizes);

    //return median file size to main
    return median;
}
#endif //VERS1_MEDIANATOR_H
