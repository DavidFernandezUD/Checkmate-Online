#include <stdio.h>
#include <string.h>
#include "server.h"

#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_CREDENTIALS_LENGTH 100

int checkCredentials(const char *username, const char *password) {
    char line[MAX_CREDENTIALS_LENGTH];
    char *usernameL;
    char *passwordL;
    
    FILE *file = fopen("src/server/serverconfig.txt", "r");

    if (file == NULL) {
        printf("Error opening the config file.\n");
        return 0;
    }

    while (fgets(line, MAX_CREDENTIALS_LENGTH, file) != NULL) {
        usernameL = strtok(line, ":");
        passwordL = strtok(NULL, ":");

        passwordL[strcspn(passwordL, "\n")] = 0; // Remove the line break from the password

        if (strcmp(usernameL, username) == 0 && strcmp(passwordL, password) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void requestCredentials() {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];

    // Request the username
    printf("Please, enter your username: ");
    fgets(username, MAX_USERNAME_LEN, stdin);
    username[strcspn(username, "\n")] = 0; // Remove the line break from the username

    // Request the password
    printf("Please, enter your password: ");
    fgets(password, MAX_PASSWORD_LEN, stdin);
    password[strcspn(password, "\n")] = 0; // Remove the line break from the password

    if (checkCredentials(username, password)) {
        printf("Valid credentials. Access granted.\n");
    } else {
        printf("Invalid credentials. Access denied.\n");
    }
}

int main() {
    requestCredentials();

    return 0;
}