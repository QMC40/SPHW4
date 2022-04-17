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
static long median = 0;
bool above = false;

bool fileXerox(const char *from_path, const char *to_path) {

    //open source file object
    FILE *fromFile = fopen(from_path, "r");
    if (!fromFile) {
        perror("failed to open source file\n");
        return false;
    }
    //open destination file object
    FILE *toFile = fopen(to_path, "w");
    if (!toFile) {
        perror("failed to open destination file\n");
        fclose(fromFile);
        return false;
    }
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    //read from source file and write to destination file
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fromFile)) > 0) {
        if (fwrite(buffer, 1, bytes_read, toFile) != bytes_read) {
            perror("failed to write to destination file\n");
            return false;
        }
    }

    //pull source permissions and set destination permissions to match
    struct stat fromFileStat;
    stat(from_path, &fromFileStat);
    chmod(to_path, fromFileStat.st_mode);

    //close files
    fclose(fromFile);
    fclose(toFile);
    return true;
}

int copyControl(const char *from_path, const struct stat *sbuf, int type,
                struct FTW *ftwb) {
    //make a string for the destination path (set max length to 256) and copy
    //dest path to it
    char to_path[PATH_MAX];
    sprintf(to_path, "%s/%s", destDir, from_path + strlen(sourceDir) + 1);

    //handle directories
    if (type == FTW_D) {
        //base directory, skip  it otherwise creates the directory and ignore error if it already exists
        if (ftwb->level == 0) {
            return 0;
        }
        if (mkdir(to_path, sbuf->st_mode) == -1) {
            //ignore error if due to directory already exists
            if (errno == EEXIST) {
                return 0;
            }
            perror("failed to mkdir: \n");
            return -1;
        }
    } else {
        //handle files
        // determine if this process is doing highs or lows, skip anything outside assigned range
        if (above) {
            if (sbuf->st_size > median) {
                if (!fileXerox(from_path, to_path)) {
                    return -1;
                }
            }
        } else {
            if (sbuf->st_size < median) {
                if (!fileXerox(from_path, to_path)) {
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

    //start file walker with source directory w/out de-referencing soft links
    return (nftw(source, copyControl, 64, FTW_PHYS) == 0) ? 0 : -1;
}

#endif //VERS1_COPIER_H
