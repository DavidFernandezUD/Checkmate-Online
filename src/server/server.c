#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_CREDENTIALS_LENGTH 100

// TODO: Read config file without "string.h"
// TODO: Improve config file

int checkCredentials(const char *username, const char *password) {
    char line[MAX_CREDENTIALS_LENGTH];
    char *usernameL;
    char *passwordL;
    
    // Open the config file
    FILE *file = fopen("config/serverconfig.txt", "r");

    // Check if the file exists
    if (file == NULL) {
        printf("Error opening the config file.\n");
        return 0;
    }

    // Check if the credentials coincide with the ones in the config file
    while (fgets(line, MAX_CREDENTIALS_LENGTH, file) != NULL) {
        usernameL = strtok(line, ":");
        passwordL = strtok(NULL, ":");

        passwordL[strcspn(passwordL, "\n")] = 0; // Remove the line break from the password

        // Compare the introduced username and password with the ones in the config file
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
    if (fgets(username, MAX_USERNAME_LEN, stdin) != NULL) {
        username[strcspn(username, "\n")] = 0; // Remove the line break from the username
    } else {
        fprintf(stderr, "Error reading user input\n");
        exit(1);
    }
    

    // Request the password
    printf("Please, enter your password: ");
    if (fgets(password, MAX_PASSWORD_LEN, stdin)) {
        password[strcspn(password, "\n")] = 0; // Remove the line break from the password
    } else {
        fprintf(stderr, "Error reading user input\n");
        exit(1);
    }

    // Accept access in case of correct username and password and vice versa
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