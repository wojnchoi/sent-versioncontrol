#include<dirent.h>
#include"sent.h"

static char *rand_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}
char * sha1_to_zipname(unsigned char *sha1)
{
	static char buffer[50];
	static const char hex[] = "0123456789abcdef";
	char *buf = buffer;
	int i;

	for (i = 0; i < 20; i++) {
		unsigned int val = *sha1++;
		*buf++ = hex[val >> 4];
		*buf++ = hex[val & 0xf];
	}
    
	return buffer;
}
char *sha1filename() {
    SHA_CTX c;
    time_t t;
    srand((unsigned) time(&t));
    char *sha1 = (char*)malloc(sizeof(char)*20);
    const size_t bufsize = rand() % 10;
 	char *buf = (char*)malloc(sizeof(char) * bufsize);
    rand_string(buf, bufsize);
    SHA1_Init(&c);
    SHA1_Update(&c,buf, bufsize);
    SHA1_Final(sha1, &c);
    return sha1_to_zipname(sha1);
}
int makeObject(const char *sha1) {
    FLIST flist;
    vector_setup(&flist, 10, sizeof(fileList));
    readFileList(".", &flist);
    
    zipInit(sha1, APPEND_STATUS_CREATE);
    zipMake("./", &flist);
    zipFinish(zip_);

    vector_clear(&flist);
    vector_destroy(&flist);
    printf("object finished\n");
    return 0;
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
int getIndexFile(char t, char buf[]) {
	int fd = open(INDEX_FILE, O_RDONLY, 0600);
    if(fd < 0)
		error_print("cannot open index");

    int back = t - '0';
    lseek(fd, -21*back, SEEK_END);
    read(fd, buf, 21);
    buf[20] = '\0';
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