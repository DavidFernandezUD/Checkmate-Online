#include "server_database.h"

// Just to test the "load_users" function
User users[] = {
    {"User1", 7, 2},
    {"User2", 4, 0},
    {"User3", 10, 4}
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

        // Execute the selection query
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
        sprintf(insert_query, "INSERT INTO USERS (username, matches_played, matches_won) VALUES ('%s', %d, %d);",
                users[i].username, users[i].matches_played, users[i].matches_won);

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
        {1, "BOT",},
        {2, "BOT"},
        {2, "PLAYER"}
};
int n_matches = sizeof(matches) / sizeof(matches[0]);

// Function to add matches to the database
int load_matches(sqlite3* db, Match* matches, int n_matches) {
    for (int i = 0; i < n_matches; i++) {
        char select_query[512];
        sprintf(select_query, "SELECT COUNT(*) FROM MATCHES WHERE user_id=%d AND winner='%s';",
                matches[i].user_id, matches[i].winner);

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
            return 1;
        }

        // Execute the selection query
        int count = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        // If the match already exists, go to the next one
        if (count > 0) {
            fprintf(stderr, "\e[0;33m[WARNING]\e[0m Match already exists in the database\n");
            continue;
        }

        // Insert the match into the database
        char insert_query[512];
        sprintf(insert_query, "INSERT INTO MATCHES (user_id, winner) "
                              "VALUES (%d, '%s');",
                              matches[i].user_id, matches[i].winner);

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
    // TODO: add password and creation_date
    const char* create_users_table = 
        "CREATE TABLE IF NOT EXISTS USERS ("
        "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL UNIQUE,"
        "matches_played INTEGER NOT NULL DEFAULT 0,"
        "matches_won INTEGER NOT NULL DEFAULT 0"
        ");";
    
    // Create table MATCHES
    // TODO: add date
    const char *create_matches_table = 
        "CREATE TABLE IF NOT EXISTS MATCHES ("
        "match_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "winner TEXT CHECK(winner IN ('PLAYER', 'BOT', 'DRAW')) NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES USERS (user_id) ON DELETE CASCADE"
        ");";

    // Create table MOVEMENTS
    const char *create_movements_table = 
        "CREATE TABLE IF NOT EXISTS MOVEMENTS ("
        "movement_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "movement_n INTEGER NOT NULL,"
        "movement TEXT NOT NULL,"
        "match_id INTEGER NOT NULL,"
        "FOREIGN KEY (match_id) REFERENCES MATCHES (match_id) ON DELETE CASCADE"
        ");";

    /*
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
    */

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

    /*
    // CHATS TABLE
    rc = sqlite3_exec(*db, create_chats_table, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table CHATS: %s\n", sqlite3_errmsg(*db));
        return 1;
    }
    */

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
        "+----+----------+----------------+--------------+---------+\n"
        "| ID | Username | Matches Played | Matches Won  | Winrate |\n"
        "+----+----------+----------------+--------------+---------+\n"
    );
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int user_id = sqlite3_column_int(stmt, 0);
        const unsigned char* username = sqlite3_column_text(stmt, 1);
        int matches_played = sqlite3_column_int(stmt, 2);
        int matches_won = sqlite3_column_int(stmt, 3);
        double winrate = (matches_played > 0) ? ((double)matches_won / matches_played) * 100.0 : 0.0;

        printf(
            "| %-2d | %-8s | %-14d | %-12d | %-7.2f |\n",
            user_id,
            username,
            matches_played,
            matches_won,
            winrate
        );
    }
    printf("+----+----------+----------------+--------------+---------+\n\n");
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
        "+----+----------+------------+\n"
        "| ID | User ID  |   Winner   |\n"
        "+----+----------+------------+\n"
    );
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int match_id = sqlite3_column_int(stmt, 0);
        int user_id = sqlite3_column_int(stmt, 1);
        const unsigned char* winner = sqlite3_column_text(stmt, 2);

        printf("| %-2d | %-8d | %-10s |\n", match_id, user_id, winner);
    }
    printf("+----+----------+------------+\n\n");
    sqlite3_finalize(stmt);
}

int update_user_parameter(sqlite3* db, int user_id, const char* parameter, const char* new_value) {
    const char* column_name;
    const char* value_format;
    if (strcmp(parameter, "username") == 0) {
        column_name = "username";
        value_format = "'%s'"; // Valor de cadena
    } else if (strcmp(parameter, "matches_played") == 0 || strcmp(parameter, "matches_won") == 0) {
        column_name = parameter;
        value_format = "%d"; // Valor entero
    } else {
        fprintf(stderr, "Invalid parameter: %s\n", parameter);
        return 1;
    }

    char update_query[512];
    sprintf(update_query, "UPDATE USERS SET %s=? WHERE user_id=?", column_name);

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, update_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing update query: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    if (strcmp(value_format, "'%s'") == 0) {
        sqlite3_bind_text(stmt, 1, new_value, -1, SQLITE_STATIC);
    } else {
        int int_value = atoi(new_value);
        sqlite3_bind_int(stmt, 1, int_value);
    }
    sqlite3_bind_int(stmt, 2, user_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed updating user parameter in USERS table: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_finalize(stmt);
    return 0;
}


int user_exists(sqlite3* db, int user_id) {
    char select_query[512];
    sprintf(select_query, "SELECT COUNT(*) FROM USERS WHERE user_id=%d;", user_id);

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
        return 0; // Indica un error, el usuario no existe
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    // Si count es mayor que 0, significa que el usuario existe
    return count > 0 ? 1 : 0;
}

// Delete selected row from the database
int delete_rows(sqlite3* db, const char* table, const char* condition) {
    char delete_query[512];
    sprintf(delete_query, "DELETE FROM %s WHERE %s;", table, condition);

    // Ejecutar la consulta de eliminación
    int rc = sqlite3_exec(db, delete_query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed deleting rows from table %s: %s\n", table, sqlite3_errmsg(db));
        return 1;
    }
    return 0;
}

// Save a new match
int save_match(sqlite3* db, const char* date, int user_id, const char* winner) {
    
    // Comprobar si el partido ya existe en la base de datos
    char select_query[512];
    sprintf(select_query, "SELECT COUNT(*) FROM MATCHES WHERE date='%s' AND user_id=%d;", date, user_id);

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Ejecutar la consulta de selección
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    // Si el partido ya existe, devolver un error
    if (count > 0) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Match already exists in the database.\n");
        return 1;
    }

    // Insertar el partido en la base de datos
    char insert_query[512];
    sprintf(insert_query, "INSERT INTO MATCHES (date, user_id, winner) "
                          "VALUES ('%s', %d, '%s');",
                          date, user_id, winner);

    rc = sqlite3_exec(db, insert_query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed inserting data into MATCHES table: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    return 0;
}


// Save a new movement
int save_movement(sqlite3* db, int match_id, int movement_n, const char* movement) {
    // Insertar el movimiento en la base de datos
    char insert_query[512];
    sprintf(insert_query, "INSERT INTO MOVEMENTS (movement_n, movement, match_id) "
                          "VALUES (%d, '%s', %d);",
                          movement_n, movement, match_id);

    int rc = sqlite3_exec(db, insert_query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed inserting data into MOVEMENTS table: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    return 0;
}


int save_username(sqlite3* db, const char* username) {
    int result = 0; // Variable para almacenar el resultado de la operación

    // Check if the username already exists in the database
    sqlite3_stmt* stmt;
    const char* select_query = "SELECT COUNT(*) FROM USERS WHERE username=?";
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
        return -1; // Devuelve -1 en caso de error
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    // Execute the selection query
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    // If the username already exists, return 0
    if (count > 0) {
        printf("\e[0;33m[WARNING]\e[0m User %s already exists in the database\n", username);
        return 0;
    }

    // Insert the user in the database
    const char* insert_query = "INSERT INTO USERS (username) VALUES (?)";
    rc = sqlite3_prepare_v2(db, insert_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing insertion statement: %s\n", sqlite3_errmsg(db));
        return -1; // Devuelve -1 en caso de error
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    // Execute the insertion query
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed inserting data into USERS table: %s\n", sqlite3_errmsg(db));
        return -1; // Devuelve -1 en caso de error
    }

    printf("Username %s saved successfully\n", username);
    result = 1; // La operación se realizó correctamente, devuelve 1
    return result;
}


// Show statistics of current user
void show_user_statistics(sqlite3* db, const char* username) {
    // Build the select query
    char select_query[512];
    sprintf(select_query, "SELECT * FROM USERS WHERE username='%s';", username);

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, select_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed preparing statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n\e[1;97mUser Statistics:\e[0m\n");
    printf(
        "+----+----------+------------+------+---------------+---------+-----------------+-----------------+\n"
        "| ID | Username |  Password  | ELO  | Creation Date | Winrate | Winrate (white) | Winrate (black) |\n"
        "+----+----------+------------+------+---------------+---------+-----------------+-----------------+\n"
    );
    if (sqlite3_step(stmt) == SQLITE_ROW) {
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