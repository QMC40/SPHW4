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

static char *destDir;
static char *sourceDir;
static bool copy_busy = false;
static long median = 0;
bool above = false;

bool copy_file(const char *from_path, const char *to_path) {
        while (copy_busy) {
            printf("copy is busy\n");
            int p = 0;
            while(p != 10000) {
                p++;
            }
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

int cp_callback(const char *fpath, const struct stat *sb, int typeflag,
                struct FTW *ftwb) {
    char to_path[PATH_MAX];
    sprintf(to_path, "%s/%s", destDir, fpath + strlen(sourceDir) + 1);

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
                if (!copy_file(fpath, to_path)) {
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
                if (!copy_file(fpath, to_path)) {
                    return -1;
                }
            }
        }
    }
    return 0;
}

//copy source
int directoryCopier(char *source, char *destination, long med, bool high) {
    median = med;
    above = high;

    destDir = destination;
    sourceDir = source;

    return (nftw(source, cp_callback, 64, FTW_PHYS) == 0) ? 0 : -1;
}

#endif //VERS1_COPIER_H
