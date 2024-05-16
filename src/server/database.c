#include "database.h"

// TODO add ON DELETE CASCADE to the MATCHES table

// Just to test the "load_users" function
User users[] = {
    {"User1", "password1", 1500, "2024-04-04", 0.6, 0.5, 0.7},
    {"User2", "password2", 1600, "2024-04-04", 0.7, 0.6, 0.8},
    {"User3", "password3", 1700, "2024-04-04", 0.8, 0.7, 0.9}
};
int n_users = sizeof(users) / sizeof(users[0]);

// Function to add users to the database
int load_users(sqlite3* db, User* users, int n_users) {
    for (int i = 0; i < n_users; i++) {   
        char select_query[512];
        sprintf(select_query, "SELECT COUNT(*) FROM USERS WHERE username='%s';", users[i].username);

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
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
            printf("\e[0;33m[WARNING]\e[0m User %s already exists in the database\n", users[i].username);
            continue;
        }

        // Insert the user in the database
        char insert_query[512];
        sprintf(insert_query, "INSERT INTO USERS (username, password, ELO, creation_date, winrate, winrate_white, winrate_black) "
                              "VALUES ('%s', '%s', %d, '%s', %f, %f, %f);",
                              users[i].username, users[i].password, users[i].ELO, users[i].creation_date,
                              users[i].winrate, users[i].winrate_white, users[i].winrate_black);

        // Execute the insertion query
        rc = sqlite3_exec(db, insert_query, 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed inserting data into USERS table: %s\n", sqlite3_errmsg(db));
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
        sprintf(select_query, "SELECT COUNT(*) FROM MATCHES WHERE date='%s' AND black_user_id=%d AND white_user_id=%d;",
                matches[i].date, matches[i].black_user_id, matches[i].white_user_id);

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
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
            fprintf(stderr, "\e[0;33m[WARNING]\e[0m Match already exists in the database\n");
            continue;
        }

        // Insert the user in the database
        char insert_query[512];
        sprintf(insert_query, "INSERT INTO MATCHES (date, black_user_id, white_user_id, match_type) "
                              "VALUES ('%s', %d, %d, '%s');",
                              matches[i].date, matches[i].black_user_id, matches[i].white_user_id, matches[i].match_type);

        // Execute the insertion query
        rc = sqlite3_exec(db, insert_query, 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed inserting data into MATCHES table: %s\n", sqlite3_errmsg(db));
            return 1;
        }
    }

    return 0;
}

// Open or create database
int initialize_db(sqlite3** db) {
    int rc = sqlite3_open("database.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m failed opening the database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return 1;
    }

    // Create table USERS
    const char* create_users_table = 
        "CREATE TABLE IF NOT EXISTS USERS ("
        "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
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
        "black_user_id INTEGER,"
        "white_user_id INTEGER,"
        "match_type TEXT,"
        "FOREIGN KEY (black_user_id) REFERENCES JUGADORES (user_id),"
        "FOREIGN KEY (white_user_id) REFERENCES JUGADORES (user_id)"
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
        "sender_user_id INTEGER,"
        "receiver_user_id INTEGER,"
        "FOREIGN KEY (match_id) REFERENCES PARTIDAS (match_id),"
        "FOREIGN KEY (sender_user_id) REFERENCES JUGADORES (user_id),"
        "FOREIGN KEY (receiver_user_id) REFERENCES JUGADORES (user_id)"
        ");";

    // Execute table creation queries
    // USERS TABLE
    rc = sqlite3_exec(*db, create_users_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed creating table USERS: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    load_users(*db, users, n_users);
    // MATCHES TABLE
    rc = sqlite3_exec(*db, create_matches_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed creating table MATCHES: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    load_matches(*db, matches, n_matches);
    // MOVEMENTS TABLE
    rc = sqlite3_exec(*db, create_movements_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed creating table MOVEMENTS: %s\n", sqlite3_errmsg(*db));
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
    const char* select_query = "SELECT * FROM USERS;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n\e[1;97mRegistered Users:\e[0m\n");
    printf(
        "+----+----------+------------+------+---------------+---------+-----------------+-----------------+\n"
        "| ID | Username |  Password  | ELO  | Creation Date | Winrate | Winrate (white) | Winrate (black) |\n"
        "+----+----------+------------+------+---------------+---------+-----------------+-----------------+\n"
    );
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        const unsigned char* password = sqlite3_column_text(stmt, 2);
        int elo = sqlite3_column_int(stmt, 3);
        const unsigned char* creation_date = sqlite3_column_text(stmt, 4);
        double winrate = sqlite3_column_double(stmt, 5);
        double winrate_white = sqlite3_column_double(stmt, 6);
        double winrate_black = sqlite3_column_double(stmt, 7);

        printf(
            "| %-2d | %-8s | %-10s | %d |  %s   |  %.2f   |      %.2f       |      %.2f       |\n",
            user_id,
            username,
            password,
            elo,
            creation_date,
            winrate,
            winrate_white,
            winrate_black
        );
    }
    printf("+----+----------+------------+------+---------------+---------+-----------------+-----------------+\n\n");
    sqlite3_finalize(stmt);
}

// Show in the console every match in the database
void show_matches(sqlite3* db) {
    const char* select_query = "SELECT * FROM MATCHES;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n\e[1;97mPlayed Matches:\e[0m\n");
    printf(
        "+----+------------+----------+----------+------------+\n"
        "| ID |    Date    | ID white | ID black | Match type |\n"
        "+----+------------+----------+----------+------------+\n"
    );
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int match_id = sqlite3_column_int(stmt, 0);
        const unsigned char* date = sqlite3_column_text(stmt, 1);
        int black_user_id = sqlite3_column_int(stmt, 2);
        int white_user_id = sqlite3_column_int(stmt, 3);
        const unsigned char* match_type = sqlite3_column_text(stmt, 4);

        printf("| %-2d | %s | %-8d | %-8d | %-10s |\n", match_id, date, black_user_id, white_user_id, match_type);
    }
    printf("+----+------------+----------+----------+------------+\n\n");
    sqlite3_finalize(stmt);
}

// Update parameters of the USERS table
int update_user_parameter(sqlite3* db, int user_id, const char* parameter, const char* new_value) {
    // Map of names so that SQLite understands which parameter to edit
    const char* column_name;
    if (strcmp(parameter, "name") == 0) {
        column_name = "username";
    } else if (strcmp(parameter, "password") == 0) {
        column_name = "password";
    } else if (strcmp(parameter, "elo") == 0) {
        column_name = "ELO";
    } else {
        fprintf(stderr, "Invalid parameter: %s\n", parameter);
        return 1;
    }

    // Build the update query
    char update_query[512];
    sprintf(update_query, "UPDATE USERS SET %s='%s' WHERE user_id=%d;", column_name, new_value, user_id);

    // Execute the update query
    int rc = sqlite3_exec(db, update_query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed updating user parameter in USERS table: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    return 0;
}

// Delete selected row from the database
int delete_rows(sqlite3* db, const char* table, const char* condition) {
    char delete_query[512];
    sprintf(delete_query, "DELETE FROM %s WHERE %s;", table, condition);

    // Execute the deletion query
    int rc = sqlite3_exec(db, delete_query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed deleting rows from table %s: %s\n", table, sqlite3_errmsg(db));
        return 1;
    }
    return 0;
}