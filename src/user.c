#include"sent.h"

User user;

void encrypt(int *password) {
    *password ^= key ^ 43543;
}
void decrypt(int *encrypted) {
    *encrypted ^= key ^ 43543;
}

int mkUser(const char *name, int password) {
    int i;
     char username[16];
    FILE *fp;
    if((fp = fopen(USER_CONFIG, "w")) == NULL) 
        return -1;

    strcpy(username, name);
    strcat(username,"@sent");

    encrypt(&password);
    fprintf(fp, "%s:%s:%d", name, username,password);
    fclose(fp);
    return 1;
}

struct User *getUser(int p_password)
{
     char realName[8];
     char logUsername[16];
     int password;
    FILE *fp;
    char buf[64];
    if ((fp = fopen (USER_CONFIG, "r")) == NULL)
        return NULL;

    encrypt(&p_password);
    while (fgets(buf, sizeof(buf), fp))
    {
        sscanf (buf, "%[^:]:%[^:]:%d\n", realName, logUsername, &password);
        user.username = logUsername;
        user.password = &password;
        user.name = realName;
        if (password == p_password)
        {
            fclose (fp);
            return &user;
        }
    }
    fclose (fp);
    return NULL;
}

int checkUser(int password) {
    FILE *fp;
    int fpassword;
    char name[8];
    char username[16];
    if((fp = fopen(USER_CONFIG, "r")) == NULL) 
        return 0;

    fscanf(fp, "%[^:]:%[^:]:", name, username);
    
    fseek(fp, strlen(strcat(name,username))+2, SEEK_SET);
    fscanf(fp, "%d\n", &fpassword);
    decrypt(&fpassword);
    if(password == fpassword) 
        return 1;

    return -1;
}

void displayUser(struct User *user) {
    printf("   name                :     %s\n", user->name);
    printf("   username            :     %s\n", user->username);
    printf("   password(encrypted) :     %d\n", *(user->password));
}