#include <stdio.h>

#ifndef _SERVER_H
#define _SERVER_H

// Useful admin username and password attributes
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_CREDENTIALS_LENGTH 100

// Check if itroduced admin credentials are correct
int check_credentials(const char* username, const char* password);

// Request admin credentials in the console
void requestCredentials();

#endif // _SERVER_H