#include "server.h"

#include "../chess/uci.h"
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
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed opening config file at config/serverconfig.txt\n");
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
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed reading user input\n");
        exit(1);
    }
    
    // Request the admin password
    printf("Please, enter your password: ");
    if (fgets(password, MAX_A_PASSWORD_LEN, stdin)) {
        password[strcspn(password, "\n")] = 0; // Remove the line break from the admin password
    } else {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed reading user input\n");
        exit(1);
    }

    // Accept access in case of correct admin username and password and vice versa
    if (checkCredentials(username, password)) {
        printf("Valid credentials. Access granted.\n");
        *credentialsValid = 1;
    } else {
        printf("Invalid credentials. Access denied.\n");
        *credentialsValid = 0;
    }
}


// Ask and update parameters of the USERS table
void update_user(sqlite3* db) {
    
    printf("Enter the ID of the user you want to edit: ");
    int user_id;
    scanf("%d", &user_id);

    printf("Which parameter do you want to edit? (name, password, elo): ");
    char parameter[MAX_PARAMETER_LENGTH];
    scanf("%s", parameter);

    printf("Enter the new value: ");
    char new_value[MAX_VALUE_LENGTH];
    scanf("%s", new_value);

    if (update_user_parameter(db, user_id, parameter, new_value) != 0) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed updating user parameter\n");
    }
}


// Delete selected user from the database
void remove_user(sqlite3* db) {
    printf("Enter the ID of the user you want to delete: ");
    int user_id;
    scanf("%d", &user_id);
    char condition[50];
    sprintf(condition, "user_id = %d", user_id);
    if (delete_rows(db, "USERS", condition) != 0) {
        fprintf(stderr, "\e[0;31m[ERROR\e[0m deleting rows from the USERS table\n");
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
                fprintf(stderr, "\e[0;31m[ERROR]\e[0m Not a valid option\n");
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
            start_server();
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
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Not a valid option\n");
            break;
    }
}

// Start uciloop (wait for an incoming move command)
void uci_loop2(SOCKET client_socket, Position* pos) {
    const int BUFFER_LEN = 2048;
    char input_buffer[BUFFER_LEN];
    char output_buffer[BUFFER_LEN];

    // Enviar el mensaje de bienvenida al cliente
    sprintf(output_buffer, "id CHESS-E\n");
    send(client_socket, output_buffer, strlen(output_buffer), 0);
    sprintf(output_buffer, "uciok\n");
    send(client_socket, output_buffer, strlen(output_buffer), 0);

    while (1) {
        // Limpiar el buffer de entrada antes de cada lectura
        memset(input_buffer, 0 , sizeof(input_buffer));
        
        // Esperar a recibir un mensaje del cliente
        int bytes_received = recv(client_socket, input_buffer, BUFFER_LEN, 0);
        if (bytes_received <= 0) {
            // Manejar errores de recepción o cierre de conexión
            break;
        }

        // Procesar el comando recibido del cliente
        if (strncmp(input_buffer, "isready", 7) == 0) {
            // Preparado para recibir comandos
            sprintf(output_buffer, "readyok\n");
            send(client_socket, output_buffer, strlen(output_buffer), 0);
        } else if (strncmp(input_buffer, "position", 8) == 0) {
            // Procesar la posición recibida
            parse_position(pos, input_buffer);
        } else if (strncmp(input_buffer, "go", 2) == 0) {
            // Procesar el comando "go"
            parse_go(pos, input_buffer);
            
            // Aquí deberías llamar a la lógica del motor de ajedrez para obtener el mejor movimiento
            // Por ahora, simplemente enviamos un movimiento de ejemplo
            sprintf(output_buffer, "bestmove e2e4\n");
            send(client_socket, output_buffer, strlen(output_buffer), 0);
        } else if (strncmp(input_buffer, "quit", 4) == 0) {
            // Comando de salida, terminar el bucle
            break;
        } else if (strncmp(input_buffer, "uci", 3) == 0) {
            // Comando UCI, responder con la identificación
            sprintf(output_buffer, "id CHESS-E\n");
            send(client_socket, output_buffer, strlen(output_buffer), 0);
            sprintf(output_buffer, "uciok\n");
            send(client_socket, output_buffer, strlen(output_buffer), 0);
        }
    }

    // Cerrar el socket del cliente al salir del bucle
    closesocket(client_socket);
}
