#ifndef OBJECT_H
#define OBJECT_H

#include"../lib/cvector.h"

typedef struct {
    char sha[20]; /* inbox */
    char obj[20]; /* object */
    bufstrType *msg;
}commit;

/* write */
extern int updateIndexFile(char *sha1);
extern int makeObject();

/* read */
extern int getIndexFile(int back);
extern int restoreObject();

/* file */
typedef struct {
    char path_[32];
    struct tm time_;
}fileList;
typedef Vector FLIST;
extern int readFileList(const char *folder, FLIST *mlist);
extern int printFileList(FLIST *list);
#endif /* object.h */