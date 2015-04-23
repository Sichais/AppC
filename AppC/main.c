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
#include <time.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>

//Struct User========BEGIN=======
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
//Struct User========END==========

//Struct pokemon=====BEGIN========
typedef struct Type {
    char type;
    int weakNormal;
    int weakFighting;
    int weakFlying;
    int weakPoison;
    int weakGhost;
    int weakElectric;
    int weakPsychic;
    int weakDragon;
    int weakDark;
    int weakGround;
    int weakRock;
    int weakWater;
    int weakBug;
    int weakSteel;
    int weakFire;
    int weakGrass;
    int weakIce;
} TYPE;
struct pokemon {
    char   *pokeName;
    TYPE    type;
    char   *moveOne;
    char   *moveTwo;
    char   *moveThree;
    char   *moveFour;
    int     HP;
    int     attack;
    int     defense;
    int     spAttack;
    int     spDefense;
    int     speed;
};
struct pokemon *sendOut
(char *pokeName, TYPE *type, char *moveOne, char *moveTwo, char *moveThree, char *moveFour,
 int HP, int attack, int defense, int spAttack, int spDefense, int speed) {
    struct pokemon *who = malloc(sizeof(struct pokemon));
    assert(who != NULL);
    
    who -> pokeName  = strdup(pokeName);
    who -> type      = *type;
    who -> moveOne   = strdup(moveOne);
    who -> moveTwo   = strdup(moveTwo);
    who -> moveThree = strdup(moveThree);
    who -> moveFour  = strdup(moveFour);
    
    who -> HP        = HP;
    who -> attack    = attack;
    who -> defense   = defense;
    who -> spAttack  = spAttack;
    who -> spDefense = spDefense;
    who -> speed     = speed;
    
    return who;
}
void removePokemon(struct pokemon *who) {
    assert (who != NULL);
    
    free(who -> pokeName);
    free(who -> moveOne);
    free(who -> moveTwo);
    free(who -> moveThree);
    free(who -> moveFour);
    
    free(who);
}
int main(int argc, const char * argv[]) {
    //TYPES, TYPES EVERYWHERE=======================
    //Normal, Fight, Fly, Poison, Ghost, Elec, Psy, Dragon, Dark, Ground, Rock, Water, Bug, Steel, Fire, Grass, Ice
    TYPE Cater  = {1, 0, 2, 1, 1, 1, 1, 1, 1, 0, 2, 1, 1, 1, 2, 1, 1};
    TYPE Bulb   = {1, 0, 2, 1, 1, 0, 2, 1, 1, 1, 1, 0, 1, 1, 2, 0, 2};
    TYPE Char   = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 0, 0, 0};
    TYPE Squir  = {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 0, 0, 2, 0};
    TYPE Darude = {1, 1, 1, 0, 1, 3, 1, 1, 1, 1, 0, 2, 1, 1, 1, 2, 2};
    TYPE Macho  = {1, 1, 2, 1, 1, 1, 2, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1};
    FILE *ofp, *ifp;
    time_t currentTime;
    char user[21];
    char pass[21];
    char logCheck;
    bool breakOut = false;
    ifp = fopen("UserList.txt", "r");
    //Password cloak structs
    struct termios oflags, nflags;
    
    printf("Welcome to this Application\nLog in? y/n: ");
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
    int     userSich = 0;
    int     passSich = 1;
    int     adminSich = 2;
    bool    loggedIn = 0;
    ofp = fopen("/Users/Sichais/Documents/UserInfo.txt", "w");
    struct User *mainSession = SessionCreate(user, pass, "0");
    while (fgets(line, sizeof(line), ifp)) {
        if (count == userSich) {
            strncpy(pastUser, line, sizeof(line));
            pastUser[strcspn(pastUser, "\n")] = 0;
        } else if (count == passSich) {
            strncpy(pastPass, line, sizeof(line));
            pastPass[strcspn(pastPass, "\n")] = 0;
        } else if (count == adminSich) {
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
        time(&currentTime);
        int pokemonChoose;
        int actionChoose;
        int moveSelect;
        int itemSelect;
        int pokeSelect;

        bool isBulb, isChar, isSquir, isDarude, isVulpix, isMacho, isDone;
        printf("Welcome! The time is currently: %s\nOh no a wild Caterpie appeared, who do you send out?: "
               "\n1. Bulbasaur."
               "\n2. Charmander."
               "\n3. Squirtle. "
               "\n4. Sandshrew. "
               "\n5. Vulpix. "
               "\n6. Machop. \n Select using 1-6, please: ", ctime(&currentTime));
        struct pokemon *Caterpie = sendOut("Caterpie", &Cater, "Tackle", "String Shot", "Bug Bite", "\0", 45, 30, 35, 20, 20, 45);
        scanf("%20d", &pokemonChoose);
        getchar();
        do {
            switch (pokemonChoose) {
                case 1:
                    ++isBulb;
                    ++isDone;
                    break;
                case 2:
                    ++isChar;
                    ++isDone;
                    break;
                case 3:
                    ++isSquir;
                    ++isDone;
                    break;
                case 4:
                    ++isDarude;
                    ++isDone;
                    break;
                case 5:
                    ++isVulpix;
                    ++isDone;
                    break;
                case 6:
                    ++isMacho;
                    ++isDone;
                    break;
                default:
                    printf("Enter a number between One (1) and Six (6): ");
                    break;
                }
        } while (!isDone); isDone = 0;
        if (isBulb != 0) {
            struct pokemon *Bulbasaur = sendOut("Bulbasaur", &Bulb, "Tackle", "Growl", "Leech Seed", "Vine Whip",
                                                45, 49, 49, 65, 65, 45);
            int tacklePP = 35;
            int LSPP     = 10;
        do {
            printf("Select an Option: \n1. Fight\t\t2. Item\n3. Pokemon\t\t4. Run\n");
            do {
                scanf("%20d", actionChoose);
                getchar();
                if (actionChoose == 1) {
                    
                } else if (actionChoose == 2) {
                    
                } else if (actionChoose == 3) {
                    
                } else if (actionChoose == 4) {
                    
                }
            } while (!isDone); --isDone;
        } while (!isDone);
            
        } else if (isChar != 0) {
            struct pokemon *Charmander = sendOut("Charmander", &Char, "Scratch", "Growl", "Ember", "Smokescreen",
                                                 39, 52, 43, 60, 50, 65);
        } else if (isSquir != 0) {
            struct pokemon *Squirtle = sendOut("Squirtle", &Squir, "Tackle", "Tail Whip", "Water Gun", "Withdraw",
                                               44, 48, 65, 50, 64, 43);
        } else if (isDarude != 0) {
            struct pokemon *Sandshrew = sendOut("Sandshrew", &Darude, "Scratch", "Defense Curl", "Sand Attack", "Poison Sting",
                                                50, 75, 85, 20, 30, 40);
        } else if (isVulpix != 0) {
            struct pokemon *Vulpix = sendOut("Vulpix", &Char, "Ember", "Tail Whip", "Roar", "Baby-Doll Eyes",
                                             38, 41, 40, 50, 65, 65);
        } else if (isMacho != 0) {
            struct pokemon *Machop = sendOut("Machop", &Macho, "Low Kick", "Leer", "Focus Energy", "Karate Chop",
                                             70, 80, 50, 35, 35, 35);
        }
    }
    fprintf(ofp, "%s \n%s \n%d", mainSession -> userName, mainSession -> password, mainSession -> isAdmin);
    return 0;
}
