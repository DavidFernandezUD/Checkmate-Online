#include <iostream>
#include <winsock2.h> 

#include "client.h"

int main() {
    SOCKET s = connect_to_server();
    if (s == INVALID_SOCKET) {
        return 1;
    }

    char choice;

    // Main menu
    do {
        show_main_menu();
        std::cout << "Choose an option: ";
        std::cin >> choice;
        handle_main_menu_option(choice, s);
    } while (choice != 'q');
    
    return 0;
}