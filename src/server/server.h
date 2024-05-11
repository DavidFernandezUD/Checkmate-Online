#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h> 
#include "../../lib/sqlite/sqlite3.h"


#ifndef _SERVER_H
#define _SERVER_H


// Request admin credentials in the console
void requestCredentials(int* credentialsValid);

// Display main menu options
void show_main_menu();

// Handle user input in main menu
void handle_main_menu_option(sqlite3* db, char choice);

// Initialize server
void start_server();


#endif // _SERVER_H
