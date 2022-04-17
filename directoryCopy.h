#ifndef VERS1_DIRECTORYCOPY_H
#define VERS1_DIRECTORYCOPY_H


#include <dirent.h>
#include <ftw.h>
#include <libgen.h>
#include "tlpi_hdr.h"
//void listFiles(const char *dirpath)
//{
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
//    while ((dp = readdir(dirp)) != NULL) {
//        errno = 0; /* To distinguish error from end-of-directory */
//        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
//            continue; /* Skip . and .. */
//        if (!isCurrent)
//            printf("test %s/", dirpath);
//        printf("%s\n", dp->d_name);
//    }
//    if (errno != 0)
//        errExit("readdir");
//    if (closedir(dirp) == -1)
//        errMsg("closedir");
//}
//
//int tester(const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
//{
//    if(S_ISDIR(sb->st_mode)) {
//        char *name = basename(path);
//        printf("%s is a directory\n", (name));
//
//    } else {
//        printf("%s is a file\n", (basename(path)));
//    }
//    return 0;
//}
//
//void dirTest(const char *dirpath,const char *dest) {
//    if(nftw(dirpath, tester, 10, 0) != 0) {
//        errExit("unable to complete walking file tree for adding file sizes");
//    }
//}

#endif //VERS1_DIRECTORYCOPY_H
