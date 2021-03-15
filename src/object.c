#include"sent.h"
#include<dirent.h>
int makeObject() {

}
int updateIndexFile(char *sha1) {
	int fd;

	fd = open(INDEX_FILE, O_RDWR | O_APPEND | O_CREAT, 0600);
	if(fd < 0)
		error_print("cannot open index");

    strcat(sha1, "\n");
	write(fd, sha1, strlen(sha1));
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

int readFileList(const char *folder, FLIST *mlist) {
    int i = 0;

    fileList flist;
    struct dirent **namelist;
    int fd;
    fd = scandir(folder, &namelist, NULL, alphasort);
    if(fd < 0)
        perror("scandir");
    else
    while(i < fd){
        if(!strcmp(namelist[i]->d_name, ".sent") || !strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name,"..")) {
            i++; continue;
        }

        struct stat st;
        strcpy(flist.path_,folder);
        strcat(flist.path_, "/");
        strcat(flist.path_, namelist[i]->d_name);

        if(namelist[i]->d_type == DT_DIR) {
            readFileList(flist.path_, mlist);
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
        vector_push_back(mlist, &flist);
        i++;
    }
    return 0;
}
int printFileList(FLIST *list) {
        VECTOR_FOR_EACH(list, i) {
        fileList path = ITERATOR_GET_AS(fileList, &i);
        printf("%s\n", path.path_);
	}
    return 0;
}