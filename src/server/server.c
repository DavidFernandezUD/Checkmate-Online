#include <string.h>
#include <time.h>
#include "server.h"
#include "../chess/uci.h"
#include "../logger.h"
// gcc src/server/database.c src/server/server_main.c src/server/server_socket.c src/server/server.c lib/sqlite/sqlite3.c src/chess/attack.c src/chess/bitboard.c src/chess/eval.c src/chess/makemove.c src/chess/move.c src/chess/movegen.c src/chess/perftest.c src/chess/random.c src/chess/search.c src/chess/uci.c -lm -lws2_32 -o bin/server
// TODO: Improve config file

// Check if itroduced admin credentials are correct
int checkCredentials(const char *username, const char *password) {
    
    char line[MAX_A_CREDENTIALS_LENGTH];
    char* tag;
    char* value;
    char* usernameL;
    char* passwordL;
    
    // Open the admin config file
    FILE* file = fopen("config/serverconfig.txt", "r");

    // Check if the admin config file
    if (file == NULL) {
        log("logs/server.log", "\e[0;31m[ERROR]\e[0m Failed opening config file at config/serverconfig.txt\n");
        exit(1);
    }

    // Check if the admin credentials coincide with the ones in the config file
    while (fgets(line, MAX_A_CREDENTIALS_LENGTH, file) != NULL) {

        usernameL = strtok(line, ":");
        passwordL = strtok(NULL, ":");

        passwordL[strcspn(passwordL, "\n")] = 0; // Remove the line break from the admin password

        // Compare the introduced admin username and password with the ones in the config file
        if (strcmp(usernameL, username) == 0 && strcmp(passwordL, password) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Request admin credentials in the console
void requestCredentials(int* credentialsValid) {

    char username[MAX_A_USERNAME_LEN];
    char password[MAX_A_PASSWORD_LEN];

    // Request the admin username
    printf("Please, enter your username: ");
    if (fgets(username, MAX_A_USERNAME_LEN, stdin) != NULL) {
        username[strcspn(username, "\n")] = 0; // Remove the line break from the admin username
    } else {
        log("logs/server.log", "\e[0;31m[ERROR]\e[0m Failed reading user input\n");
        exit(1);
    }
    
    // Request the admin password
    printf("Please, enter your password: ");
    if (fgets(password, MAX_A_PASSWORD_LEN, stdin)) {
        password[strcspn(password, "\n")] = 0; // Remove the line break from the admin password
    } else {
        log("logs/server.log", "\e[0;31m[ERROR]\e[0m Failed reading user input\n");
        exit(1);
    }

    // Accept access in case of correct admin username and password and vice versa
    if (checkCredentials(username, password)) {
        log("logs/server.log", "[INFO] Valid credentials, access granted\n");
        printf("Valid credentials. Access granted.\n");
        *credentialsValid = 1;
    } else {
        log("logs/server.log", "[INFO] Invalid credentials, access denied\n");
        printf("Invalid credentials. Access denied.\n");
        *credentialsValid = 0;
    }
}

// Ask and update parameters of the USERS table
void update_user(sqlite3* db) {
    printf("Enter the ID of the user you want to edit: ");
    int user_id;
    if (scanf("%d", &user_id) != 1) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Invalid user ID.\n");
        return;
    }

    printf("Which parameter do you want to edit? (username, matches_played, matches_won): ");
    char parameter[MAX_PARAMETER_LENGTH];
    if (scanf("%s", parameter) != 1) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed to read parameter.\n");
        return;
    }

    printf("Enter the new value: ");
    char new_value[MAX_VALUE_LENGTH];
    if (scanf("%s", new_value) != 1) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed to read new value.\n");
        return;
    }

    if (update_user_parameter(db, user_id, parameter, new_value) != 0) {
        log("logs/server.log", "\e[0;31m[ERROR]\e[0m Failed updating user parameter\n");
        return;
    }

    printf("User parameter updated successfully.\n");
}

// Delete selected user from the database
void remove_user(sqlite3* db) {
    printf("Enter the ID of the user you want to delete: ");
    int user_id;
    scanf("%d", &user_id);

    if (!user_exists(db, user_id)) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m User with ID %d does not exist.\n", user_id);
        return;
    }

    char condition[50];
    sprintf(condition, "user_id = %d", user_id);

    if (delete_rows(db, "USERS", condition) != 0) {
        log("logs/server.log", "\e[0;31m[ERROR\e[0m deleting rows from the USERS table\n");
    }
}

// Handle user managing menu options
void manage_users_menu(sqlite3* db) {
    char choice;
    do {
        show_users(db);
        printf("---------\n");
        printf("e -> Edit User\n");
        printf("r -> Remove User\n");
        printf("q -> Go Back\n");
        printf("Choose an option: ");
        scanf(" %c", &choice);
        switch (choice) {
            case 'e':
                update_user(db);
                break;
            case 'r':
                remove_user(db);
                break;
            case 'q':
                break;
            default:
                log("logs/server.log", "\e[0;31m[ERROR]\e[0m Not a valid option\n");
                break;
        }
    } while (choice != 'q');
}


// Show options in the main menu
void show_main_menu() {
    printf("\nMain Menu\n");
    printf("---------\n");
    printf("s -> Run Server\n");
    printf("u -> Manage Users\n");
    printf("m -> Show Matches\n");
    printf("q -> Quit\n");
}


// Handle main menu options
void handle_main_menu_option(sqlite3* db, char choice) {
    switch (choice) {
        case 's':
            start_server(db);
            break;
        case 'u':
            manage_users_menu(db);
            break;
        case 'm':
            show_matches(db);
            break;
        case 'q':
            break;
        default:
            log("logs/server.log", "\e[0;31m[ERROR]\e[0m Not a valid option\n");
            break;
    }
}

int makePlayerMove(Position* pos, char* moves, char* move, unsigned int* n_move) {

    char* temp_moves = (char*) malloc(2048 * sizeof(char));

    strcpy(temp_moves, moves);

    strcat(temp_moves, move);
    strcat(temp_moves, " ");

    if (parse_position(pos, temp_moves) == 0) {
        strcpy(moves, temp_moves);
        free(temp_moves);
        (*n_move)++;
        return 0;
    }

    free(temp_moves);
    return ERR_ILEGAL_MOVE;
}

void makeEngineMove(Position* pos, char* moves, int depth, unsigned int* n_move) {

    int best_move;
    int half_moves = 0;
    int nodes = 0;

    int score = negmax(pos, depth, -99999, 99999, &half_moves, &nodes, &best_move);

    char engine_move[16];
    if (GET_MOVE_PROMOTION(best_move)) {
        sprintf(engine_move, "%s%s%c", square_to_coordinates[GET_MOVE_SRC(best_move)], square_to_coordinates[GET_MOVE_DST(best_move)], ASCII_PIECES[GET_MOVE_PROMOTION(best_move)]);
    } else {
        sprintf(engine_move, "%s%s", square_to_coordinates[GET_MOVE_SRC(best_move)], square_to_coordinates[GET_MOVE_DST(best_move)]);
    }
    
    strcat(moves, engine_move);
    strcat(moves, " ");

    parse_position(pos, moves);
    (*n_move)++;
}

// Start uciloop (wait for an incoming move command)
void uci_loop2(SOCKET client_socket, sqlite3* db, const char* username, Position* pos)  {

    const int BUFFER_LEN = 2048;
    char input_buffer[BUFFER_LEN];

    // Initialize game
    unsigned int n_move;
    char moves[2048];
    Color playerColor;

    n_move = 0;
    strcpy(moves, "position startpos");
    parse_position(pos, moves);
    
    // Randomize player color
    srand(time(NULL));
    playerColor = (rand() % 2 == 0) ? WHITE : BLACK;

    strcat(moves, " moves ");

    if (playerColor == BLACK) {
        
        // Engine does first move
        makeEngineMove(pos, moves, 6, &n_move);

        // Send color to player
        send(client_socket, "You play as BLACK\n", 19, 0);
    } else {
        // Send color to player
        send(client_socket, "You play as WHITE\n", 19, 0);
    }

    send_position_to_client(client_socket, *pos);

    while (1) {

        // Reset user input buffer
        memset(input_buffer, 0, sizeof(input_buffer));

        // Wait to receive a message from the client
        int bytes_received = recv(client_socket, input_buffer, BUFFER_LEN, 0);
        if (bytes_received <= 0) {
            // Handle reception or connection closure errors
            break;
        }

        // Parse UCI commands received from the client
        if (makePlayerMove(pos, moves, input_buffer, &n_move) == ERR_ILEGAL_MOVE) {
            send(client_socket, "Ilegal move, try again!\n", 25, 0);
            continue;
        }
        print_position(*(pos));

        MoveList list;

        // Check that the player hasn't won
        generate_moves(pos, &list);
        if (list.top != 0) {

            // Engine makes move after player
            makeEngineMove(pos, moves, 6, &n_move);
        
            // Check that engine hasn't won the game
            generate_moves(pos, &list);
            if (list.top == 0) {
                int user_id = get_user_id(db, username);
                increment_matches(db, username);
                send(client_socket, "Engine won!\n", 13, 0);
                save_match(db, user_id, "BOT");
                break;
            }

        } else {
            int user_id = get_user_id(db, username);
            increment_matches(db, username); // Incrementa partidas jugadas
            increment_wins(db, username); 
            send(client_socket, "Player won!\n", 13, 0);
            save_match(db, user_id, "PLAYER");
            break;
        }

        send_position_to_client(client_socket, *pos);
    }

    // Close client socket on loop exit
    closesocket(client_socket);
}

// Send chessboard status to client
void send_position_to_client(SOCKET client_socket, Position position) {
    char position_buffer[2048]; // Buffer size for storing the position

    // Construct the position string in the buffer
    int offset = 0;
    for (int i = 0; i < 8; i++) {
        // Add row number at the beginning of each line
        offset += snprintf(position_buffer + offset, sizeof(position_buffer) - offset, " %d ", 8 - i);
        
        for (int j = 0; j < 8; j++) {
            int square = i * 8 + j;

            int piece = -1;
            for (int k = 0; k < 12; k++) {
                if (GET_BIT(position.bitboards[k], square)) {
                    piece = k;
                    break;  // Stop once the piece is found
                }
            }

            char piece_char = (piece == -1) ? '.' : ASCII_PIECES[piece];
            offset += snprintf(position_buffer + offset, sizeof(position_buffer) - offset, " %c", piece_char);
        }
        position_buffer[offset++] = '\n';
    }

    // Add column labels
    offset += snprintf(position_buffer + offset, sizeof(position_buffer) - offset, "\n    a b c d e f g h\n\n");

    // Add additional information to the buffer (turn, castling, enpassant)
    snprintf(position_buffer + offset, sizeof(position_buffer) - offset,
             "Turn:      %s\n"
             "Castling:  %c%c%c%c\n"
             "Enpassant: %s\n\n",
             (position.turn == WHITE) ? "white" : "black",
             (position.castling & WK) ? 'K' : '-',
             (position.castling & WQ) ? 'Q' : '-',
             (position.castling & BK) ? 'k' : '-',
             (position.castling & BQ) ? 'q' : '-',
             (position.enpassant != NULL_SQUARE) ? square_to_coordinates[position.enpassant] : "no");

    // Send the position to the client via the socket
    send(client_socket, position_buffer, strlen(position_buffer), 0);
}
