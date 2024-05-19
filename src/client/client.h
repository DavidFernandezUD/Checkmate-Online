#include <iostream>

#include "client_socket.h"
#include "user.h"

#ifndef _CLIENT_H
#define _CLIENT_H

// Print main menu options
void show_main_menu();

// Handle different main menu options
void handle_main_menu_option(char choice, SOCKET s);

#endif // _CLIENT_H