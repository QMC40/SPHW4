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

int compare(const void *a, const void *b) {
    return (*(long *) a > *(long *) b) ? 1 : -1;
}

void printArray(long A[], int size) {
    for (i = 0; i < size; i++) {
        printf("%ld ", A[i]);
    }
    printf("\n");
}

int directoryPrint(const char *pathname, const struct stat *sbuf, int type,
                   struct FTW *ftwb) {
    switch (sbuf->st_mode & S_IFMT) { /* Print file type */
        case S_IFDIR:
            break;
//        case S_IFREG:
//            totalSize += sbuf->st_size;
//            fileCount++;
    }
    printf(" %*s", 4 * ftwb->level, ""); /* Indent suitably */
    printf("%s\n", &pathname[ftwb->base]); /* Print basename */

    return 0; /* Tell nftw() to continue */
}

//take the list of file sizes, sort and select and return median file size
long medianFinder() {
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
    printf("median is: %ld @ %.1f\n", median,(fileCount-1) / 2.0);
    return median;
}

//transit the directory tree and count files
int counter(const char *dirpath, const struct stat *sbuf, int type,
            struct FTW *ftwb) {
    if (S_ISREG(sbuf->st_mode)) {
        fileCount++;
        totalSize += sbuf->st_size;
    }
    return 0;
}

int stacker(const char *dirpath, const struct stat *sbuf, int type,
            struct FTW *ftwb) {
    if (S_ISREG(sbuf->st_mode)) {
        fileSizes[i] = sbuf->st_size;
        i++;
    }
    return 0; /* Tell nftw() to continue */
}

void folderCrawler(const char *dirpath) {
    //error check this later!!!
    //prints listing of directory supplied w/ *dirpath
    nftw(dirpath, directoryPrint, 10, 0);
    //iteration counter set to 0 and count files in directory tree
    i = 0;
    nftw(dirpath, counter, 10, 0);
//    printf("file count: %d - total size: %ld\n", fileCount, totalSize);
    //create an array large enough to hold all file sizes, exit if unable to create dynamic array
    fileSizes = (long *) calloc(fileCount, sizeof(long));
    if (fileSizes == NULL) {
        perror("unable to create file listing");
        exit(1);
    }

    //reset counter and
    i = 0;
    nftw(dirpath, stacker, 10, 0);
    medianFinder();
    //release dynamic array used to find median
    free(fileSizes);
}
#endif //VERS1_MEDIANATOR_H
