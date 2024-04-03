#include <stdio.h>
#include "server.h"
#include "database.h"

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
        fprintf(stderr, "Failed to initialize the database.\n");
        return 1;
    }

    char choice;

    // Main menu
    do {
        show_menu();
        printf("Choose an option: ");
        scanf(" %c", &choice); // Read user's choice

        switch (choice) {
            case 'u':
                printf("See users\n");
                break;
            case 'm':
                printf("Show matches\n");
                break;
            case 'q':
                printf("Quit\n");
                break;
            default:
                printf("Not a valid option\n");
                break;
        }

    } while (choice != 'q');

    // Close database
    close_database(db);
    
    return 0;
}