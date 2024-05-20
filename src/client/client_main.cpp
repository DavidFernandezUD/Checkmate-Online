#include "client.h"

int main() {
    SOCKET s = connect_to_server();
    if (s == INVALID_SOCKET) {
        return 1;
    }

    User user;
    std::string username;

    std::cout << "Enter your username: ";
    std::cin >> username;

    user.setUsername(username);

    // send username to the server
    std::cout << std::endl << "Welcome to Checkmate Online, " << user.getUsername() << "!" << std::endl << std::endl;

    send_username_to_server(s, user.getUsername());


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
