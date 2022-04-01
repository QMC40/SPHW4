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

unsigned int copyfile(char *source[], char *dest[], int runs, int buffer_Size) {
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

        clock_t start = clock();

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

        clock_t stop = clock();
        total += (unsigned long) (((double) (stop - start) * 1000) / CLOCKS_PER_SEC);

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
