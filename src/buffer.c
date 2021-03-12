#include"sent.h"

bufstrType *bufferNew(char* data) {
    bufstrType *newbuf = (bufstrType*)malloc(sizeof(bufstrType));
    if(data != NULL) {
        int len = strlen(data);
        newbuf->size = (len < MAX_BUFFER_SIZE) ? MAX_BUFFER_SIZE : len;
        newbuf->used = len;
        newbuf->buffer = malloc(newbuf->size * sizeof(char));
        strncpy(newbuf->buffer,data,len+1);
    } else {
        newbuf->size = MAX_BUFFER_SIZE;
        newbuf->used = 0;
        newbuf->buffer = malloc(newbuf->size * sizeof(char));
    }
    return newbuf;
}

void bufferDelete(bufstrType *buf) {
    free(buf->buffer);
    free(buf);
}

int bufferLength(bufstrType *buf) {
    return (buf->used);
}

void bufferAddRoom(bufstrType *buf, int size) {
    if(buf->size <= size) {
        int newSize = buf->size * 2;
        if(newSize < size)
            newSize += size;
        buf->buffer = realloc(buf->buffer,newSize + 1);
        buf->size = newSize;
    }
}

void bufferAddChar(bufstrType *buf, char data) {
    bufferAddRoom(buf, buf->used +1);
    buf->buffer[buf->used] = data;
    buf->used++;
    buf->buffer[buf->used] = '\0';
}

void bufferSet(bufstrType *buf, const char *str) {
    int len = strlen(str);
    bufferAddRoom(buf, len+1);
    strncpy(buf->buffer, str, len+1);
    buf->used = len;
    buf->buffer[buf->used] = '\0';
}

void buffer_addN(bufstrType *buf, const char *str, int len) {
    int end = buf->used;
    bufferAddRoom(buf, buf->used +len+1);
    strncpy(buf->buffer + end, str, len+1);
    buf->used += len;
    buf->buffer[buf->used] = '\0';
}
void bufferAdd(bufstrType *buf, char *str) {
    int len = strlen(str);
    buffer_addN(buf, str, len);
}

void bufferPrepend(bufstrType *buf, char *str) {
    int len = strlen(str);
    int end = buf->used;
    bufferAddRoom(buf,buf->used+len+1);
    for(int i = end; i >= 0; i--) {
        buf->buffer[i+len] = buf->buffer[i];
    }
    strncpy(buf->buffer, str,len);
    buf->used += len;
}

void bufferAddAll(bufstrType *buf, int n, ...) {
    char *s;
    va_list ap;
    va_start(ap, n);
    for(int i = 0; i<n; i++) {
        s = va_arg(ap,char*);
        bufferAdd(buf,s);
    }
    va_end(ap);
}

void bufferPrintf(bufstrType *buf, char *format, ...) {
    va_list ap;
    int n, size = 100;
    while(1) {
        bufferAddRoom(buf, size+1);
        va_start(ap, format);
        n = vsnprintf(buf->buffer, size, format, ap);
        va_end(ap);
        if(n > -1 && n < size) {
            buf->used = n;
            return;
        }
        size *= 2;
    }
}

void bufferAddPrintf(bufstrType *buf, char *format,...) {
    va_list ap;
    int n , size = 100;
    int end = buf->used;
    while(1) {
        bufferAddRoom(buf,end+size+1);
        va_start(ap, format);
        n = vsnprintf(buf->buffer+end, size, format, ap);
        va_end(ap);
        if(n > -1 && n < size) {
            buf->used = end+n;
            return;
        }
        size *= 2;
    }
}

char *bufferGetCopy(bufstrType *buf) {
    char *result = malloc(buf->size +1);
    strncpy(result, buf->buffer,buf->used);
    result[buf->used] = '\0';
    return result;
}
char *bufferGetRef(bufstrType *buf) {
    return buf->buffer;
}
char *bufferGetBuffer(bufstrType *buf) {
    char *result = buf->buffer;
    buf->size = MAX_BUFFER_SIZE;
    buf->buffer = malloc(buf->size * sizeof(char));
    buf->used = 0;
    return result;
}

void bufferReset(bufstrType *buf) {
    buf->used = 0;
    buf->buffer[0] = '\0';
}