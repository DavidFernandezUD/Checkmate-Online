#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

// TODO: Improve config file

// Check if itroduced admin credentials are correct
int checkCredentials(const char *username, const char *password) {
    char line[MAX_CREDENTIALS_LENGTH];
    char *usernameL;
    char *passwordL;
    
    // Open the admin config file
    FILE *file = fopen("config/serverconfig.txt", "r");

    // Check if the admin config file
    if (file == NULL) {
        printf("Error opening the config file.\n");
        return 0;
    }

    // Check if the admin credentials coincide with the ones in the config file
    while (fgets(line, MAX_CREDENTIALS_LENGTH, file) != NULL) {
        usernameL = strtok(line, ":");
        passwordL = strtok(NULL, ":");

        passwordL[strcspn(passwordL, "\n")] = 0; // Remove the line break from the admin password

        // Compare the introduced admin username and password with the ones in the config file
        if (strcmp(usernameL, username) == 0 && strcmp(passwordL, password) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Request admin credentials in the console
void requestCredentials(int* credentialsValid) {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];

    // Request the admin username
    printf("Please, enter your username: ");
    if (fgets(username, MAX_USERNAME_LEN, stdin) != NULL) {
        username[strcspn(username, "\n")] = 0; // Remove the line break from the admin username
    } else {
        fprintf(stderr, "Error reading user input\n");
        exit(1);
    }
    

    // Request the admin password
    printf("Please, enter your password: ");
    if (fgets(password, MAX_PASSWORD_LEN, stdin)) {
        password[strcspn(password, "\n")] = 0; // Remove the line break from the admin password
    } else {
        fprintf(stderr, "Error reading user input\n");
        exit(1);
    }

    // Accept access in case of correct admin username and password and vice versa
    if (checkCredentials(username, password)) {
        printf("Valid credentials. Access granted.\n");
        *credentialsValid = 1;
    } else {
        printf("Invalid credentials. Access denied.\n");
        *credentialsValid = 0;
    }
}

// Show options in the menu
void show_menu() {
    printf("Press:\n");
    printf("u -> see users\n");
    printf("m -> see matches\n");
    printf("q -> quit\n");
}