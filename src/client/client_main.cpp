#include <iostream>

#include "client.h"

int main() {

    char choice;

    // Main menu
    do {
        show_main_menu();
        printf("Choose an option: ");
        scanf(" %c", &choice); // Read user's choice
        handle_main_menu_option(choice);
    } while (choice != 'q');
    
    return 0;
}