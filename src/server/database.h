#include "../../lib/sqlite/sqlite3.h"

#ifndef DATABASE_H
#define DATABASE_H

// Useful username and password attributes
#define MAX_P_USERNAME_LENGTH 50
#define MAX_P_PASSWORD_LENGTH 50

// User struct
typedef struct {
    char username[MAX_P_USERNAME_LENGTH];
    char password[MAX_P_PASSWORD_LENGTH];
    int ELO;
    char creation_date[20];
    float winrate;
    float winrate_white;
    float winrate_black;
} User;

// Match struct
typedef struct {
    char date[20];
    int black_user_id;
    int white_user_id;
    char match_type[20];
} Match;

// Open/create and close database
int initialize_db(sqlite3** db);
void close_database(sqlite3* db);

// Show users in the console
void show_users(sqlite3* db);
// Show matches in the console
void show_matches(sqlite3* db);

// Delete selected row from the database
int delete_rows(sqlite3* db, const char* table, const char* condition);

// Update parameters of the USERS table
int update_user_parameter(sqlite3* db, int user_id, const char* parameter, const char* new_value);

#endif