#ifndef BUFFER_H
#define BUFFER_H
#define MAX_BUFFER_SIZE 128

typedef struct bufstrType {
    char *buffer;
    unsigned size;
    unsigned used;
}bufstrType;

extern bufstrType *bufferNew(char* data);

extern int bufferLength(bufstrType *buf);

extern void bufferAddRoom(bufstrType *buf, int size);

extern void bufferAddChar(bufstrType *buf, char data);

extern void bufferSet(bufstrType *buf, const char *str);

extern void buffer_addN(bufstrType *buf, const char *str, int len);

extern void bufferAdd(bufstrType *buf, char *str);
extern void bufferAddAll(bufstrType *buf, int n, ...);

extern void bufferPrepend(bufstrType *buf, char *str);

extern void bufferPrintf(bufstrType *buf, char *format, ...);
extern void bufferAddPrintf(bufstrType *buf, char *format,...);

extern char *bufferGetCopy(bufstrType *buf);
extern char *bufferGetRef(bufstrType *buf);
extern char *bufferGetBuffer(bufstrType *buf);

extern void bufferReset(bufstrType *buf);

extern void bufferDelete(bufstrType *buf);

#endif /* buffer.h */