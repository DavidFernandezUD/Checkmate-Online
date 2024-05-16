#include <iostream>

#include "client_socket.h"

#ifndef _CLIENT_H
#define _CLIENT_H

void show_main_menu();

void handle_main_menu_option(char choice, SOCKET s);

#endif // _CLIENT_H