#ifndef OBJECT_H
#define OBJECT_H
#include<openssl/sha.h> 
#include"../lib/minizip/include/zip.h"
#include"cvector.h"

/* file */
typedef struct {
    char path_[32];
    struct tm time_;
}fileList;
typedef Vector FLIST;
extern int readFileList(const char *folder, FLIST *mlist);
extern int printFileList(FLIST *list);

zipFile zip_;
extern int zipInit(const char *filename, int append);
extern int zipMake(const char *root, FLIST *mlist);
extern int zipFinish();

extern char *sha1filename();
/* write */
extern int updateIndexFile(char *sha1);
// extern int makeObject();

/* read */
extern int getIndexFile(char t, char buf[]);
// extern int restoreObject();

#endif /* object.h */