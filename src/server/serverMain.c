#include <stdio.h>
#include "server.h"
#include "database.h"

int main() {
    // Request and check admin credentials in the console
    requestCredentials();
    
    // Initialize database
    sqlite3 *db;
    if (initialize_db(&db) != 0) {
        fprintf(stderr, "Failed to initialize the database.\n");
        return 1;
    }
    return 0;
}