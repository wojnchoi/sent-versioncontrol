#include"sent.h"

typedef enum 
{mail, send, back, config}cmd; //add cmd

cmd find_command(char *arg) {
    char *commands[] = {"mail", "send", "back", "config"};
    for(int i = 0; i<CMD_CNT; i++)
        if(!strcmp(commands[i], arg))
            return i;
    return -1;
}
static const char *const usg[] = {
    "usage: ./sent [command]\n\n",
    "command\n",
    "   mail - create repository\n",
    "   send - save files to repository\n",
    "   back - back files to previous(specific) repository\n",
    "   config - edit user information.\n", //add options later
    NULL
};
static void usage(const char *const *msg) {
    for(; *msg; msg++)
        fprintf(stderr,WHT " \t %s", *msg);
    exit(1);
}
void error_print(const char *msg) {
    fprintf(stderr, RED "%s\n", msg);
    exit(1);
}


int main(int argc, char *argv[]) {
    cmd command;
    int password;

    if(argc < 2)
        usage(usg);

    command = find_command(argv[1]);
    if(command == 0) { //init .sent dir
        char name[16];
        if(mkdir(SENT_DIR, 0700) < 0) {
            struct stat a;
            if(stat(SENT_DIR, &a) == 0 && S_ISDIR(a.st_mode))
                printf(CYN "%s exists. already initialized\n", SENT_DIR);
            else
                error_print("failed to initialize");

            return 0;
        }
        if(mkdir(DEFAULT_REPO, 0700) < 0) {
            if(errno != EEXIST) {
                perror(DEFAULT_REPO);
                exit(1);
            }
        }
        printf("%s init done\n", SENT_DIR);
        printf("USER CONFIG SETTING: %s\n", USER_CONFIG);
        printf("NAME: "); scanf("%s[^\n]", name);
        printf("PASSWORD: "); scanf("%d[^\n]", &password);
        if(mkUser(name, password) != 1)
            error_print("failed to create user");
        printf("created user, path: %s\n", USER_CONFIG);   
    } else {
        struct stat a;
        stat(SENT_DIR, &a);
        if(!S_ISDIR(a.st_mode))
            error_print("init sent first..");
        
        if(command == 1) { //send
            updateCache();
            send_cmd();
        } else if(command == 2) { //back
            //checkHASH
            //back_cmd();
        } else if(command == 3) { //config
            printf("password: "); scanf(" %d", &password);
            if(checkUser(password) != 1)
                error_print("wrong password");

            User *user = getUser(password);
            if(user != NULL)
                displayUser(user);
        } else {
            printf(RED "check your command!\n");
            usage(usg);
        }

    }
}