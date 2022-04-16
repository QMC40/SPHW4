/*
###########################################################
# Aaron Fortner                                           #
# COSC 4348-W01                                           #
# Systems Programming                                     #
# Assignment 4 - threaded file copy                       #
###########################################################
 */
#ifndef VERS1_COPIER_H
#define VERS1_COPIER_H

#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>



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

unsigned int copier(char *source[], char *dest[], int runs, int buffer_Size) {
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    int count = runs;
    unsigned long total = 0;

    char *buff;
    buff = (char *) malloc(buffer_Size * sizeof(char));
    if (buff == NULL) {
        printf("unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    while (count) {
        inputFd = open(*source, O_RDONLY);
        if (inputFd == -1) {
            perror("error opening source file");
            exit(EXIT_FAILURE);
        }
        openFlags = O_CREAT | O_WRONLY | O_TRUNC;
        filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                    S_IROTH | S_IWOTH; /* rw-rw-rw- */
        outputFd = open(*dest, openFlags, filePerms);
        if (outputFd == -1) {
            perror("error opening destination file");
            exit(EXIT_FAILURE);
        }

        while ((numRead = read(inputFd, buff, buffer_Size)) > 0) {
            if (numRead == -1) {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            if (write(outputFd, buff, numRead) != numRead) {
                perror("unable to write whole buffer");
                exit(EXIT_FAILURE);
            }
        }

        if (close(inputFd) == -1) {
            perror("error closing source file");
            exit(EXIT_FAILURE);
        }
        if (close(outputFd) == -1) {
            perror("error closing destination file");
            exit(EXIT_FAILURE);
        }
        count--;
    }

    unsigned long ms = total / runs;
    free(buff);
    return ms;
}

#endif //VERS1_COPIER_H
