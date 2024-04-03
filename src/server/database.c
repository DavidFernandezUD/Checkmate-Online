#include <stdio.h>
#include <stdlib.h>
#include "../../lib/sqlite/sqlite3.h"

// Open or create database
int initialize_db(sqlite3** db) {
    int rc = sqlite3_open("database.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening the database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return 1;
    }

    // Create table USERS
    const char* create_players_table = 
        "CREATE TABLE IF NOT EXISTS PLAYERS ("
        "player_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "ELO INTEGER,"
        "creation_date TEXT,"
        "winrate REAL,"
        "winrate_white REAL,"
        "winrate_black REAL"
        ");";
    
    // Create table MATCHES
    const char *create_matches_table = 
        "CREATE TABLE IF NOT EXISTS MATCHES ("
        "match_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "date TEXT,"
        "black_player_id INTEGER,"
        "white_player_id INTEGER,"
        "match_type TEXT,"
        "FOREIGN KEY (black_player_id) REFERENCES JUGADORES (player_id),"
        "FOREIGN KEY (white_player_id) REFERENCES JUGADORES (player_id)"
        ");";

    // Create table MOVEMENTS
    const char *create_movements_table = 
        "CREATE TABLE IF NOT EXISTS MOVEMENTS ("
        "movement_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "movement_n INTEGER,"
        "movement TEXT,"
        "match_id INTEGER,"
        "movement_time TEXT,"
        "FOREIGN KEY (match_id) REFERENCES PARTIDAS (match_id)"
        ");";

    // Create table CHATS
    const char *create_chats_table = 
        "CREATE TABLE IF NOT EXISTS CHATS ("
        "chat_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "chat_n INTEGER,"
        "chat TEXT,"
        "match_id INTEGER,"
        "chat_time TEXT,"
        "sender_player_id INTEGER,"
        "receiver_player_id INTEGER,"
        "FOREIGN KEY (match_id) REFERENCES PARTIDAS (match_id),"
        "FOREIGN KEY (sender_player_id) REFERENCES JUGADORES (player_id),"
        "FOREIGN KEY (receiver_player_id) REFERENCES JUGADORES (player_id)"
        ");";

    // Ejecutar consultas de creación de tablas
    rc = sqlite3_exec(*db, create_players_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table PLAYERS: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    rc = sqlite3_exec(*db, create_matches_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table MATCHES: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    rc = sqlite3_exec(*db, create_movements_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table MOVEMENTS: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    rc = sqlite3_exec(*db, create_chats_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table CHATS: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    return 0;
}

// Close database
void close_database(sqlite3* db) {
    sqlite3_close(db);
}
