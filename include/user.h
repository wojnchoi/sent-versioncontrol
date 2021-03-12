#ifndef USER_H
#define USER_H

#ifndef TEST
#define USER_CONFIG "./.sent/config"
#else
#define USER_CONFIG "../.sent/config"
#endif

/* temporary key for password encrypt/decrypt
 * upgrade later
 */
static const unsigned short key = 52343; 

typedef struct User {
    char *name;
    char *username; /* sent@name */
    int *password; /* encrypted */
}User;


extern void encrypt(int *password);
extern void decrypt(int *encrypted);

extern int mkUser(const char *name, int password);

extern struct User *getUser(int p_password);

extern int checkUser(int password);

extern void displayUser(struct User *user);

#endif /* user.h */