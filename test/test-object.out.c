#define TEST
#include"../include/sent.h"
#include<dirent.h>

void error_print(const char *msg) {
    fprintf(stderr,"%s\n", msg);
    exit(1);
}
/*
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
} */

int readsFile(const char *folder, FLIST *mlist) {
    int i = 0;

    fileList flist;
    struct dirent **namelist;
    int fd;
    fd = scandir(folder, &namelist, NULL, alphasort);
    printf("%d\n", fd);
    if(fd < 0)
        perror("scandir");
    else
    while(i < fd){
        if(!strcmp(namelist[i]->d_name, ".sent") || !strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name,"..")) {
            i++; continue;
        }

        struct stat st;
        strcat(flist.path_ , folder);
        strcat(flist.path_, "/");
        strcat(flist.path_, namelist[i]->d_name);

        if(namelist[i]->d_type == DT_DIR) {
            readsFile(flist.path_, mlist);
            i++;
            continue;
        }
        if(stat(flist.path_, &st) != 0)
            error_print("readfile error");

        struct tm *tmp = localtime(&st.st_mtime);
        if (tmp == NULL) {
            error_print("readfeil time error");
        }
        flist.time_ = *tmp;
        cvector_push_back(mlist,&flist);
        i++;
    }
    return 0;
}

int main() {
    FLIST flist;
    readsFile(".", &flist);

    if (flist) {
		int i;
		for (i = 0; i < cvector_size(&flist); ++i) {
			printf("%s\n", flist[i].path_);
		}
	}
    cvector_free(&flist);
    printf("\nfinished\n");
    return 0;
}