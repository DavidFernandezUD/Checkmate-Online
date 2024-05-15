#include <iostream>


void show_main_menu();

void handle_main_menu_option(char choice, SOCKET s);

SOCKET connect_to_server();

void send_messages(SOCKET s);