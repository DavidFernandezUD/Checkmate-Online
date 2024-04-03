#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


const char* USER_FORMAT_OUT = "%s, %s, %f, %d\n";
// %s reads until a spce character, but in this case we need to read until ","
const char* USER_FORMAT_IN = "%24[^,], %24[^,], %lf, %d\n";
const double EUR_TO_USD = 1.18;

enum Operations{
    QUIT='q',
    LOG_IN='l',
    LOG_OUT='o',
    REGISTER='r',
    WITHDRAW='w',
    INFO='i',
    DEPOSIT='d'
};


typedef struct {
    char username[25];
    char password[25];
    double credit;
    bool isEuro;
} User;


void printUser(User user) {
    if(user.isEuro) {
        printf("user[username: %s, password: %s, credit %.2f EUR]\n", user.username, user.password, user.credit);
    } else {
        printf("user[username: %s, password: %s, credit %.2f USD]\n", user.username, user.password, user.credit * EUR_TO_USD);
    }
}


void storeUser(User user, char path[]) {
    FILE* file = fopen(path, "a+");
    if(file == NULL) {
        printf("Failed opening file!\n");
        exit(1); 
    }

    fprintf(file, USER_FORMAT_OUT, user.username, user.password, user.credit, user.isEuro);

    fclose(file);
}


User readUser(FILE* file) {
    User user;

    // fscanf() returns the the number of input items successfully matched and assigned, so if it is 4 it is means it was successfull
    if(fscanf(file, USER_FORMAT_IN, user.username, user.password, &user.credit, &user.isEuro) != 4) {
        printf("Failed reading user from file!\n");
        exit(1);
    }

    return user;
}


int getUser(char username[], char path[], User* pUser) {
    
    FILE* file = fopen(path, "r+");
    if(file == NULL) {
        printf("Failed opening file!\n");
        exit(1);
    }

    // feof() returns true if the end of the file has been reached
    User user;
    while(!feof(file)) {
        user = readUser(file);

        if(strcmp(user.username, username) == 0) {
            *pUser = user;
            return 1;
        }
    }

    return 0;
}


void updateUser(const char* usernameToUpdate, const char* path, User user) {
    
    FILE* file = fopen(path, "r");
    FILE* temp = fopen("temp", "w");
    if (file == NULL || temp == NULL) {
        printf("Failed opening file!\n");
        exit(1);
    }
    
    char buffer[255];
    char username[25];
    char password[25];
    double credit;
    bool isEuro;
    while(fgets(buffer, 255, file)) {
        sscanf(buffer, USER_FORMAT_IN, username, password, &credit, &isEuro);
        
        credit = (strcmp(username, usernameToUpdate) == 0) ? user.credit : credit;

        fprintf(temp, USER_FORMAT_OUT, username, password, credit, isEuro);
    }
    
    fclose(temp);
    fclose(file);

    // Replace main file with temp file
    if(remove(path) != 0) {
        printf("Failed removeing file!\n");
        exit(1);
    }

    if(rename("temp", path) != 0) {
        printf("Failed renaming file!\n");
        exit(1);
    }
}


int logIn(char username[], char password[], char path[], User* pUser) {
    User user;
    if(getUser(username, path, &user)) {
        if(strcmp(user.password, password) == 0) {
            *pUser = user;
            return 1;
        } else {
            return 0;
        }
    } else {
        printf("User %s not found!\n", username);
        return 0;
    }
}


int main() {
    
    User user;

    char username[25];
    char password[25];

    char action;
    float quantity;

    bool running = true;
    bool logged = false;

    printf("quit -> q\nlog in -> l\nlog out -> o\nregister -> r\nwithdraw -> w\ndeposit -> d\ninfo -> i\n");
    
    char choice;
    while(running) {
        if(logged) {
            printf("%s >> ", username);
        } else {
            printf(">> ");
        }
        scanf(" %c", &choice);
        switch(choice) {
            case QUIT:
                running = false;
                
                if(logged) {
                    // Update state of user
                    updateUser(username, "users.txt", user);
                }

                break;
            case LOG_IN:
                do {
                    printf(">> Enter Username: ");
                    scanf("%s", username);
                    printf(">> Enter Password: ");
                    scanf("%s", password);
                } while(!logIn(username, password, "users.txt", &user));
                logged = true;
                printf(">> Logged successfully as %s!\n", username);
                break;
            case LOG_OUT:
                if(logged) {
                    updateUser(username, "users.txt", user);
                    logged = false;
                } else {
                    printf(">> You must log in first!\n");
                }
                break;
            case REGISTER:
                printf(">> Enter Username: ");
                scanf("%s", username);
                printf(">> Enter Password: ");
                scanf("%s", password);
                
                // Registering user
                user = (User){};
                strcpy(user.username, username);
                strcpy(user.password, password);
                user.credit = 0.0;
                user.isEuro = true;
                    
                // Store new user in file
                storeUser(user, "users.txt");
                logged = true;
                printf(">> Successfully registered as %s!\n", username);

                break;
            case WITHDRAW:
                if(logged) {
                    printf(">> Withdraw quantity: ");
                    scanf("%f", &quantity);

                    if(quantity <= user.credit) {
                        user.credit -= quantity;
                    } else {
                        printf(">> Not enough credit!\n");
                    }
                } else {
                    printf(">> You must log in first!\n");
                }
                break;
            case DEPOSIT:
                if(logged) {
                    printf(">> Deposit quantity: ");
                    scanf("%f", &quantity);
                    user.credit += quantity;
                } else {
                    printf(">> You must log in first!\n");
                }
                break;
            case INFO:
                if(logged) {
                    printf(">> ");
                    printUser(user);
                } else {
                    printf(">> You must log in first!\n");
                }
                break;
            default:
                printf(">> Invalid option\n");
        }
    }

    return 0;

}

main.c
Mostrando main.c.