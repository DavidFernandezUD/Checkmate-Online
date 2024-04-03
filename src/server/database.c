#include <stdio.h>
#include <stdlib.h>
#include "../../lib/sqlite/sqlite3.h"
#include "database.h"

// Just to test the "load_players" function
Player players[] = {
    {"Player1", "password1", 1500, "2024-04-04", 0.6, 0.5, 0.7},
    {"Player2", "password2", 1600, "2024-04-04", 0.7, 0.6, 0.8},
    {"Player3", "password3", 1700, "2024-04-04", 0.8, 0.7, 0.9}
};
int n_players = sizeof(players) / sizeof(players[0]); // Calculate the size of "players"

// Function to add players to the database
int load_players(sqlite3* db, Player* players, int n_players) {
    for (int i = 0; i < n_players; i++) {
        char select_query[512];
        sprintf(select_query, "SELECT COUNT(*) FROM PLAYERS WHERE username='%s';", players[i].username);

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            return 1;
        }

        // Exececute the selection query
        int count = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        // If the user already exists, go to the next one
        if (count > 0) {
            printf("The user %s already exists in the database.\n", players[i].username);
            continue;
        }

        // Insert the user in the database
        char insert_query[512];
        sprintf(insert_query, "INSERT INTO PLAYERS (username, password, ELO, creation_date, winrate, winrate_white, winrate_black) "
                              "VALUES ('%s', '%s', %d, '%s', %f, %f, %f);",
                              players[i].username, players[i].password, players[i].ELO, players[i].creation_date,
                              players[i].winrate, players[i].winrate_white, players[i].winrate_black);

        // Execute the insertion query
        rc = sqlite3_exec(db, insert_query, 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error inserting data into PLAYERS table: %s\n", sqlite3_errmsg(db));
            return 1;
        }
    }
    return 0;
}


// Just to test the "load_matches" function
Match matches[] = {
        {"2024-04-04", 1, 2, "Standatd"},
        {"2024-04-05", 2, 3, "Blitz"},
        {"2024-04-06", 1, 3, "Bullet"}
};
int n_matches = sizeof(matches) / sizeof(matches[0]);

// Function to add matches to the database
int load_matches(sqlite3* db, Match* matches, int n_matches) {
    for (int i = 0; i < n_matches; i++) {
        char select_query[512];
        sprintf(select_query, "SELECT COUNT(*) FROM MATCHES WHERE date='%s' AND black_player_id=%d AND white_player_id=%d;",
                matches[i].date, matches[i].black_player_id, matches[i].white_player_id);

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            return 1;
        }

        // Exececute the selection query
        int count = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        // If the user already exists, go to the next one
        if (count > 0) {
            printf("The match already exists in the database.\n");
            continue;
        }

        // Insert the user in the database
        char insert_query[512];
        sprintf(insert_query, "INSERT INTO MATCHES (date, black_player_id, white_player_id, match_type) "
                              "VALUES ('%s', %d, %d, '%s');",
                              matches[i].date, matches[i].black_player_id, matches[i].white_player_id, matches[i].match_type);

        // Execute the insertion query
        rc = sqlite3_exec(db, insert_query, 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error inserting data into MATCHES table: %s\n", sqlite3_errmsg(db));
            return 1;
        }
    }

    return 0;
}


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
    // PLAYERS TABLE
    rc = sqlite3_exec(*db, create_players_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table PLAYERS: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    load_players(*db, players, n_players);
    // MATCHES TABLE
    rc = sqlite3_exec(*db, create_matches_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table MATCHES: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    load_matches(*db, matches, n_matches);
    // MOVEMENTS TABLE
    rc = sqlite3_exec(*db, create_movements_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table MOVEMENTS: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    // CHATS TABLE
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

// Show in the console every user in the database
void show_users(sqlite3* db) {
    const char* select_query = "SELECT * FROM PLAYERS;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("Users registered:\n");
    printf("ID | Username | ELO | Creation date | Winrate | Winrate (white) | Winrate (black)\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int player_id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        int elo = sqlite3_column_int(stmt, 3);
        const unsigned char* creation_date = sqlite3_column_text(stmt, 4);
        double winrate = sqlite3_column_double(stmt, 5);
        double winrate_white = sqlite3_column_double(stmt, 6);
        double winrate_black = sqlite3_column_double(stmt, 7);

        printf("%d | %s | %d | %s | %.2f | %.2f | %.2f\n", player_id, username, elo, creation_date, winrate, winrate_white, winrate_black);
    }
    sqlite3_finalize(stmt);
}

// Show in the console every match in the database
void show_matches(sqlite3* db) {
    const char* select_query = "SELECT * FROM MATCHES;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("Matches played:\n");
    printf("ID | Date | ID white player | ID black player | Match type\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int match_id = sqlite3_column_int(stmt, 0);
        const unsigned char* date = sqlite3_column_text(stmt, 1);
        int black_player_id = sqlite3_column_int(stmt, 2);
        int white_player_id = sqlite3_column_int(stmt, 3);
        const unsigned char* match_type = sqlite3_column_text(stmt, 4);

        printf("%d | %s | %d | %d | %s\n", match_id, date, black_player_id, white_player_id, match_type);
    }
    sqlite3_finalize(stmt);
}

