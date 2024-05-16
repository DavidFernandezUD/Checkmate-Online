#include "client.h"

// g++ src/client/client_main.cpp src/client/client_socket.cpp src/client/client_socket.h src/client/client.cpp src/client/client.h -lm -lws2_32 -o bin/client

// Print main menu options
void show_main_menu() {
    std::cout << "Main Menu" << std::endl;
    std::cout << "---------" << std::endl;
    std::cout << "p -> Send messages" << std::endl;
    std::cout << "q -> Quit" << std::endl;
}

// Handle different main menu options
void handle_main_menu_option(char choice, SOCKET s) {
    switch (choice) {
        case 'p':
            if (s != INVALID_SOCKET) {
                send_messages(s);
            } else {
                std::cerr << "Not connected to the server. Please connect first." << std::endl;
            }
            break;
        case 'q':
            break;
        default:
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Not a valid option\n");
            break;
    }
}

