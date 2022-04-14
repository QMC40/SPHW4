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

int listFd;
long test[300000];
//
//int compare(const void *a, const void *b) {
//    return (*(long *) a > *(long *) b) ? 1 : -1;
//}
//
//void printArray(long A[], int size) {
//    for (i = 0; i < size; i++) {
//        printf("%ld ", A[i]);
//    }
//    printf("\n");
//}
//
///* List directories files in directory 'dirPath' */
//void listFiles(const char *dirpath) {
//
//    DIR *dirp;
//    struct dirent *dp;
//    Boolean isCurrent; /* True if 'dirpath' is "." */
//    isCurrent = strcmp(dirpath, ".") == 0;
//    dirp = opendir(dirpath);
//    if (dirp == NULL) {
//        errMsg("opendir failed on '%s'", dirpath);
//        return;
//    }
///* For each entry in this directory, print directory + filename */
//    for (;;) {
//
//        errno = 0; /* To distinguish error from end-of-directory */
//        dp = readdir(dirp);
//        if (dp == NULL) {
//            break;
//        }
//
//        if (dp->d_type == DT_DIR) {
//            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
//                continue;
//            } /* Skip . and .. */
//            if (!isCurrent) {
//                printf("%s/", dirpath);
//            }
//            printf("d-type: %s\n", dp->d_name);
//        }
//    }
//    if (errno != 0) {
//        errExit("readdir");
//    }
//    if (closedir(dirp) == -1) {
//        errMsg("closedir");
//    }
//}
//
//int directoryPrint(const char *pathname, const struct stat *sbuf, int type,
//                   struct FTW *ftwb) {
//    switch (sbuf->st_mode & S_IFMT) { /* Print file type */
//        case S_IFDIR:
//            break;
////        case S_IFREG:
////            totalSize += sbuf->st_size;
////            fileCount++;
//    }
//    printf(" %*s", 4 * ftwb->level, ""); /* Indent suitably */
//    printf("%s\n", &pathname[ftwb->base]); /* Print basename */
//
//    return 0; /* Tell nftw() to continue */
//}
//
//int listMaker() {
//    int outputFd, openFlags;
//    mode_t filePerms;
//
//    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
//    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
//                S_IROTH | S_IWOTH; /* rw-rw-rw- */
//    outputFd = open("sizelist.txt", openFlags, filePerms);
//    if (outputFd == -1) {
//        perror("error opening destination file");
//        exit(EXIT_FAILURE);
//    }
//    return outputFd;
//}
//
//int findMedian(const char *dirpath, const struct stat *sbuf, int type,
//               struct FTW *ftwb) {
//    char buffer[8] = {};
//    unsigned long temp = 0;
//    temp = sbuf->st_size;
//    sprintf(buffer, "%ld", temp);
//
//    if (S_ISREG(sbuf->st_mode)) {
//
////        printf("i: %d\n",i);
////        list[i] = sbuf->st_size;
////        if (write(listFd, buffer, 8) != 8) {
////            perror("unable to write whole buffer");
////            exit(EXIT_FAILURE);
////        }
////        write(listFd, "\n", 1);
////        printf("buffer: %s == %ld\n", buffer, sbuf->st_size);
////        totalSize += sbuf->st_size;
////        fileCount++;
////        i++;
//    }
//    return 0; /* Tell nftw() to continue */
//}
//
////take the list of file sizes, sort and select and return median file size
//long medianFinder() {
//    qsort(fileSizes, fileCount, sizeof(long), compare);
////    printArray(fileSizes, fileCount);
//    long median = 0;
//    //empty directory kickback
//    if (fileCount <= 0) { return 0; }
//    //find middle element (int div to trunc for odd count)
//    int med = (fileCount - 1) / 2;
//    if (fileCount % 2 == 0) {
//        median = (fileSizes[med] + fileSizes[med + 1]) / 2;
//    } else {
//        median = fileSizes[med];
//    }
//    //test print of result, using float div for element to test even / odd list handling
//    printf("median is: %ld @ %.1f\n", median,(fileCount-1) / 2.0);
//    return median;
//}
//
////transit the directory tree and count files
//int counter(const char *dirpath, const struct stat *sbuf, int type,
//            struct FTW *ftwb) {
//    if (S_ISREG(sbuf->st_mode)) {
//        fileCount++;
//        totalSize += sbuf->st_size;
//    }
//    return 0;
//}
//
//int stacker(const char *dirpath, const struct stat *sbuf, int type,
//            struct FTW *ftwb) {
//    if (S_ISREG(sbuf->st_mode)) {
//        fileSizes[i] = sbuf->st_size;
//        i++;
//    }
//    return 0; /* Tell nftw() to continue */
//}



int main(int argc, char *argv[]) {

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        usageErr("%s [dir...]\n", argv[0]);
    } else if (argc == 1) {/* No arguments - use current directory */
        folderCrawler(".");
    } else {
        for (argv++; *argv; argv++) {
            fileCount = 0;
            folderCrawler(*argv);
        }
    }
    return 0;
}