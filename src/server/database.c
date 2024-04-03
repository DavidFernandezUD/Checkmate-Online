#include <stdio.h>
#include <stdlib.h>
#include "lib/sqlite/sqlite3.h"

// Open or create database
int initialize_db(sqlite3** db) {
    int rc = sqlite3_open("database.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening the database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    return 0;
}

// Close database
void close_database(sqlite3* db) {
    sqlite3_close(db);
}
