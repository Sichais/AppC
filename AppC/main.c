//
//  main.c
//  AppC
//
//  Created by Zane Lassiter on 4/21/15.
//  Copyright (c) 2015 Zane Lassiter. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <string.h>
#include <termios.h>
#include <assert.h>

struct User {
    char   *userName;
    char   *password;
    char    isAdmin;
};
struct User *SessionCreate(char *userName, char *password, bool isAdmin) {
    struct User *who = malloc(sizeof(struct User));
    assert(who != NULL);
    
    who -> userName = strdup(userName);
    who -> password = strdup(password);
    who -> isAdmin  = isAdmin;
    return who;
}
void SessionDestroy(struct User *who) {
    assert (who != NULL);
    
    free(who -> userName);
    free(who -> password);
    
    free(who);
}
int main(int argc, const char * argv[]) {
    FILE *ofp, *ifp;
    char user[21];
    char pass[21];
    char logCheck;
    bool breakOut = false;
    ifp = fopen("UserList.txt", "r");
    //Password cloak structs
    struct termios oflags, nflags;
    
    printf("Welcome to this Application\n Log in? y/n: ");
    do {
        logCheck = getchar();
        getchar();
        
        switch (logCheck) {
            case 'Y':
            case 'y': {
                printf("Enter Username (20 characters max): ");
                scanf("%20s", user);
                getchar();
                
                //Password input
                tcgetattr(fileno(stdin), &oflags);
                nflags = oflags;
                nflags.c_lflag &= ~ECHO;
                nflags.c_lflag |= ECHONL;
                
                if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
                    perror("tcsetattr");
                    return EXIT_FAILURE;
                }
                
                printf("Enter Password (20 characters max): ");
                fgets(pass, sizeof(pass), stdin);
                pass[strlen(pass) - 1] = 0;
                
                if (tcsetattr(fileno(stdin), TCSANOW, &oflags)) {
                    perror("tcsetattr");
                    return EXIT_FAILURE;
                }
                ++breakOut;
                break;
            }
            case 'N':
            case 'n': {
                printf("Exitting program");
                ++breakOut;
                exit(1);
            }
            default:
                printf("Invalid Input\nLog In? y/n: ");
                break;
        }
    } while (!breakOut);
    char    pastUser[21];
    char    pastPass[21];
    char    pastAdmin[21];
    char    line[21];
    int     count = 0;
    int     userLine = 0;
    int     passLine = 1;
    int     adminLine = 2;
    bool    loggedIn = 0;
    ofp = fopen("/Users/Sichais/Documents/UserInfo.txt", "w");
    struct User *mainSession = SessionCreate(user, pass, "0");
    while (fgets(line, sizeof(line), ifp)) {
        if (count == userLine) {
            strncpy(pastUser, line, sizeof(line));
            pastUser[strcspn(pastUser, "\n")] = 0;
        } else if (count == passLine) {
            strncpy(pastPass, line, sizeof(line));
            pastPass[strcspn(pastPass, "\n")] = 0;
        } else if (count == adminLine) {
            strncpy(pastAdmin, line, sizeof(line));
            pastAdmin[strcspn(pastAdmin, "\n")] = 0;
        }
    }
    if (strcmp(mainSession -> userName, pastUser)) {
        if (strcmp(mainSession -> password, pastPass)) {
            printf("Logged in as...\n");
            if (strcmp(pastAdmin, "1")) {
                printf("Admin.\n");
                ++loggedIn;
            } else {
                printf("User.\n");
                ++loggedIn;
            }
        } else {
            printf("Invalid Password, exitting\n");
        }
    } else {
        printf("Invalid Username, exitting\n");
    }
    if (loggedIn == true) {
        printf("Welcome!\n");
    }
    fprintf(ofp, "%s \n%s \n%d", mainSession -> userName, mainSession -> password, mainSession -> isAdmin);
    return 0;
}
