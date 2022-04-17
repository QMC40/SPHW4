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
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <stdbool.h>
#include <sys/errno.h>
#include "error_functions.h"

#define BUFFER_SIZE 1024

static char *copy_to_path;
static char *copy_from_path;
static bool copy_busy = false;
static long median = 0;
bool above = false;

bool copy_file(const char *from_path, const char *to_path) {
    if (copy_busy) {
        printf("copy is busy\n");
        sleep(1);
    }

    copy_busy = true;
    FILE *ff = fopen(from_path, "r");
    if (!ff) {
        perror("failed to open source file\n");
        return false;
    }
    FILE *tf = fopen(to_path, "w");
    if (!tf) {
        perror("failed to open destination file\n");
        fclose(ff);
        return false;
    }
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, ff)) > 0) {
        if (fwrite(buffer, 1, bytes_read, tf) != bytes_read) {
            perror("failed to write to destination file\n");
            return false;
        }
    }
    fclose(ff);
    fclose(tf);
    copy_busy = false;
    return true;
}

static int cp_callback(const char *fpath, const struct stat *sb, int typeflag,
                       struct FTW *ftwb) {
    char to_path[PATH_MAX];
    sprintf(to_path, "%s/%s", copy_to_path, fpath + strlen(copy_from_path) + 1);

    if (above) {
        //handle directories
        if (typeflag == FTW_D) {
            //base directory, skip  it
            if (ftwb->level == 0) {
                return 0;
            }
            if (mkdir(to_path, sb->st_mode) == -1) {
                //if the directory already exists, skip it
                if (errno == EEXIST) {
                    return 0;
                }
                perror("failed to mkdir: \n");
                return -1;
            }
        } else {
            //handle files
            if (sb->st_size > median) {
                if(!copy_file(fpath, to_path)) {
                    return -1;
                }
            }
        }
    } else {
        //handle directories
        if (typeflag == FTW_D) {
            //base directory, skip  it
            if (ftwb->level == 0) {
                return 0;
            }
            if (mkdir(to_path, sb->st_mode) == -1) {
                //if the directory already exists, skip it
                if (errno == EEXIST) {
                    return 0;
                }
                perror("failed to mkdir: \n");
                return -1;
            }
        } else {
            //handle files
            if (sb->st_size <= median) {
            if(!copy_file(fpath, to_path)) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

//copy path
int copy_dir_contents(char *path, char *to, long med, bool high) {
    median = med;
    above = high;

    copy_to_path = to;
    copy_from_path = path;

    int ret = nftw(path, cp_callback, 64, FTW_PHYS);

    return ret == 0;
}

#endif //VERS1_COPIER_H
