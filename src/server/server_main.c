#include "server.h"
#include "../logger.h"

int main() {

    // Request and check admin credentials in the console
    int credentialsValid = 0;
    requestCredentials(&credentialsValid);
    if (!credentialsValid) {
        return 1;
    }

    // Initialize database
    sqlite3 *db;
    if (initialize_db(&db) != 0) {
        log_msg("logs/server.log", "\e[0;31m[ERROR]\e[0m Failed to initialize the database\n");
        return 1;
    }

    char choice;

    // Main menu
    do {
        show_main_menu();
        printf("Choose an option: ");
        scanf(" %c", &choice); // Read user's choice
        handle_main_menu_option(db, choice);
    } while (choice != 'q');

    // Close database
    close_database(db);
    
    return 0;
}
