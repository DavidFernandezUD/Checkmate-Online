#include "../../lib/sqlite/sqlite3.h"

#ifndef DATABASE_H
#define DATABASE_H

// Open/create and close database
int initialize_db(sqlite3** db);
void close_database(sqlite3* db);

#endif