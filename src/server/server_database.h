#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../lib/sqlite/sqlite3.h"

#define MAX_P_USERNAME_LENGTH 50
#define MAX_P_PASSWORD_LENGTH 50

#ifndef _SERVER_DATABASE_H
#define _SERVER_DATABASE_H

// User struct
typedef struct {
    char username[MAX_P_USERNAME_LENGTH];
    int matches_played;
    int matches_won;
} User;

// Match struct
typedef struct {
    int user_id;
    const char* winner;
} Match;

// Open or create database
int initialize_db(sqlite3** db);

// Close database
void close_database(sqlite3* db);

// Show users in the console
void show_users(sqlite3* db);

// Show matches in the console
void show_matches(sqlite3* db);

// Update parameters of the USERS table
int update_user_parameter(sqlite3* db, int user_id, const char* parameter, const char* new_value);

int user_exists(sqlite3* db, int user_id);

// Delete selected row from the database
int delete_rows(sqlite3* db, const char* table, const char* condition);

// Save a new match
void save_match(sqlite3* db, int user_id, const char* winner);

// Save a new move
int save_movement(sqlite3* db, int match_id, int movement_n, const char* movement);

void save_username(sqlite3* db, const char* username);

void increment_matches(sqlite3* db, const char* username);

void increment_wins(sqlite3* db, const char* username);

int get_user_id(sqlite3* db, const char* username);

#endif // _SERVER_DATABASE_H