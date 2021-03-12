#ifndef SENT_H
#define SENT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/stat.h>
#include <sys/mman.h>
#include<errno.h>
#include<unistd.h>
#include<time.h>
#include<assert.h>
#include<fcntl.h>

#include<zlib.h> //https://zlib.net/zlib_how.html
#include<openssl/sha.h> //https://docs.huihoo.com/doxygen/openssl/1.0.1c/include_2openssl_2sha_8h.html

#include"buffer.h"
#include"user.h"
#include"cache.h"

#define SENT_DIR ".sent"
#define DEFAULT_REPO ".sent/inbox"
#define INDEX_FILE ".sent/index"

#define CMD_CNT 4 //cmd numbers

extern void error_print(const char *msg);

/* 
 * terminal output colors
 * https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
*/
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

#endif /* sent.h */