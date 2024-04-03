#include <stdio.h>

#ifndef _SERVER_H
#define _SERVER_H

// Useful admin username and password attributes
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_CREDENTIALS_LENGTH 100

// Request admin credentials in the console
void requestCredentials(int* credentialsValid);

// Show options in the menu
void show_menu();

#endif // _SERVER_H