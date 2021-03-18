#include<dirent.h>

#include"sent.h"
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
        if(!strcmp(namelist[i]->d_name, ".git") ||!strcmp(namelist[i]->d_name, ".sent") || !strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name,"..")) {
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

int zipInit(const char *filename,int append) {
    zip_ = zipOpen(filename, append);
}
int zipMake(const char *root, FLIST *mlist) {
    zip_fileinfo info;
    memset(&info, 0, sizeof(zip_fileinfo));

    int len = strlen(root);
    char *buf;   
    long lSize;
    VECTOR_FOR_EACH(mlist, i) {
        fileList path = ITERATOR_GET_AS(fileList, &i);
        info.tmz_date.tm_hour = path.time_.tm_hour;
        info.tmz_date.tm_mday = path.time_.tm_mday;
        info.tmz_date.tm_min = path.time_.tm_min;
        info.tmz_date.tm_mon = path.time_.tm_mon;
        info.tmz_date.tm_sec = path.time_.tm_sec;
        info.tmz_date.tm_year = path.time_.tm_year;

        zipOpenNewFileInZip(zip_,path.path_+len,&info, NULL, 0, NULL, 0, NULL,Z_DEFLATED, Z_BEST_COMPRESSION);

        FILE *fp = fopen(path.path_, "rb");
        if(fp > 0) {
            fseek (fp , 0 , SEEK_END);
            lSize = ftell(fp);
            rewind (fp);
            buf = (char*)malloc(sizeof(char)*lSize);
            size_t ret = fread(buf,1,lSize,fp);
            if(ret != lSize) error_print("read error");
            zipWriteInFileInZip(zip_,buf,ret);
            printf("compressed : %s \n", path.path_);
            fclose(fp);
            zipCloseFileInZip(zip_);
        }
        
	}
}
int zipFinish() {
    zipClose(zip_, "");
}