#include <iostream>
#include <winsock2.h> 

#include "client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

// g++ src/client/client_main.cpp src/client/client.cpp -lm -lws2_32 -o bin/client

void show_main_menu() {
    std::cout << "Main Menu" << std::endl;
    std::cout << "---------" << std::endl;
    std::cout << "p -> Send messages" << std::endl;
    std::cout << "q -> Quit" << std::endl;
}

void handle_main_menu_option(char choice, SOCKET s) {
    switch (choice) {
        case 'p':
            if (s != INVALID_SOCKET) {
                send_messages(s);
            } else {
                std::cerr << "Not connected to the server. Please connect first." << std::endl;
            }
            break;
        case 'q':
            break;
        default:
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Not a valid option\n");
            break;
    }
}

SOCKET connect_to_server() {
	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock. Error Code: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Failed to create socket. Error Code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	 if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server. Error Code: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return INVALID_SOCKET;
    }

	 std::cout << "Connected to server: " << SERVER_IP << " (" << SERVER_PORT << ")" << std::endl;

	return s;
}

void send_messages(SOCKET s) {
    char message[512];
    std::cout << "Enter a message to send to the server ('q' to quit):" << std::endl;
    while (true) {
        std::cin.getline(message, sizeof(message));
        if (strcmp(message, "q") == 0) {
            break;
        }
        if (send(s, message, strlen(message), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed with error code : " << WSAGetLastError() << std::endl;
            return;
        }
        std::cout << "Message sent: " << message << std::endl;
    }
}