#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h> 
#include "../../lib/sqlite/sqlite3.h"

#include "server.h"
#include "database.h"

// gcc src/server/server_main.c src/server/database.c src/server/server.c lib/sqlite/sqlite3.c -lm -lws2_32 -o bin/server

// Useful admin username and password attributes
#define MAX_A_USERNAME_LEN 50
#define MAX_A_PASSWORD_LEN 50
#define MAX_A_CREDENTIALS_LENGTH 100

#define MAX_PARAMETER_LENGTH 10
#define MAX_VALUE_LENGTH 100

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

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

// Initialize server
void start_server() {
	WSADATA wsaData;
    SOCKET conn_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char recvBuff[512];

	printf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }

	printf("Initialised.\n");

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return;
    }

	printf("Socket created.\n");

	server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

	// BIND (the IP/port with socket)
    if (bind(conn_socket, (struct sockaddr*)&server,
             sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return;
    }

	printf("Bind done.\n");

	// LISTEN to incoming connections (socket server moves to listening mode)
    if (listen(conn_socket, 1) == SOCKET_ERROR) {
        printf("Listen failed with error code: %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return;
    }

	// ACCEPT incoming connections (server keeps waiting for them)
    printf("Waiting for incoming connections...\n");
    int stsize = sizeof(struct sockaddr);
    SOCKET comm_socket = accept(conn_socket, (struct sockaddr*)&client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
    if (comm_socket == INVALID_SOCKET) {
        printf("accept failed with error code : %d", WSAGetLastError());
        closesocket(conn_socket);
        WSACleanup();
        return;
    }
    printf("Incoming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
           ntohs(client.sin_port));

	// Closing the listening socket (is not going to be used anymore)
    closesocket(conn_socket);

	// RECEIVE and display data from clients
    printf("Waiting for incoming messages from client... \n");
    int bytes;
    do {
        bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        if (bytes > 0) {
            recvBuff[bytes] = '\0'; // Ensure null termination
            printf("Received message from client: %s\n", recvBuff);
        } else if (bytes == 0) {
            printf("Client disconnected.\n");
            break;
        } else {
            printf("recv error: %d\n", WSAGetLastError());
            break;
        }
    } while (bytes > 0);

	// CLOSING the socket and cleaning Winsock...
    closesocket(comm_socket);
    WSACleanup();
}