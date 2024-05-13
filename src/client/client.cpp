#include <iostream>

#include "client.h"

void show_main_menu() {
    std::cout << "Main Menu" << std::endl;
    std::cout << "---------" << std::endl;
    std::cout << "e -> View Statistics" << std::endl;
    std::cout << "p -> Find Match" << std::endl;
    std::cout << "q -> Quit" << std::endl;
}

void handle_main_menu_option(char choice) {
    switch (choice) {
        case 'e':
            std::cout << "statistics!" << std::endl;
            break;
        case 'p':
            std::cout << "Connecting to server..." << std::endl;
            break;
        case 'q':
            break;
        default:
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Not a valid option\n");
            break;
    }
}