#define TEST
#include"../include/sent.h"

void error_print(const char *msg) {
    fprintf(stderr,"%s\n", msg);
    exit(1);
}
int makeObject() {

}
int updateIndexFile(char *sha1) {
	int fd;

	fd = open(INDEX_FILE, O_RDWR | O_APPEND | O_CREAT, 0600);
	if(fd < 0)
		error_print("cannot open index");

    sha1[19] = '\n';
	write(fd, sha1, 20);
	return 0;
}

int restoreObject() {

}
int getIndexFile(int back) {
	int fd = open(INDEX_FILE, O_RDONLY, 0600);
    if(fd < 0)
		error_print("cannot open index");

    char buf[21];
    lseek(fd, -20*back, SEEK_END);
    read(fd, buf, 20);
    for(int i = 0; i< 20; i++) {
        printf("%c", buf[i]);
    }
    close(fd);
    return 0;
}

int createDir(const char *path)
{
	int len = strlen(path);
	char *buf = malloc(len + 1);
	const char *slash = path;

	while ((slash = strchr(slash+1, '/')) != NULL) {
		len = slash - path;
		memcpy(buf, path, len);
		buf[len] = 0;
		mkdir(buf, 0700);
	}
}
int createFile(const char *path)
{
	int fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0600);
	if (fd < 0) {
		if (errno == ENOENT) {
			createDir(path);
			fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0600);
		}
	}
	return fd;
}
int main(int argc, char *argv[]) {
    int back;
    scanf("%d", &back);
    getIndexFile(back);
    return 0;
}