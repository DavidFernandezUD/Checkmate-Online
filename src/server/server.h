#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../lib/sqlite/sqlite3.h"

#include "database.h"
#include "server_socket.h"

#define MAX_A_USERNAME_LEN 50
#define MAX_A_PASSWORD_LEN 50
#define MAX_A_CREDENTIALS_LENGTH 100

#define MAX_PARAMETER_LENGTH 10
#define MAX_VALUE_LENGTH 100

#ifndef _SERVER_H
#define _SERVER_H

// Request admin credentials in the console
void requestCredentials(int* credentialsValid);

// Display main menu options
void show_main_menu();

// Handle user input in main menu
void handle_main_menu_option(sqlite3* db, char choice);

#endif // _SERVER_H
