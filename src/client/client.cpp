#include <iostream>
#include <winsock2.h> 

#include "client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

// g++ src/client/client_main.cpp src/client/client.cpp -lm -lws2_32 -o bin/client

void show_main_menu() {
    std::cout << "Main Menu" << std::endl;
    std::cout << "---------" << std::endl;
    std::cout << "e -> View Statistics" << std::endl;
    std::cout << "p -> Find Match" << std::endl;
    std::cout << "q -> Quit" << std::endl;
}

void handle_main_menu_option(char choice) {
    switch (choice) {
        case 'e':
            std::cout << "statistics!" << std::endl;
            break;
        case 'p':
            std::cout << "Connecting to server..." << std::endl;
            break;
        case 'q':
            break;
        default:
            fprintf(stderr, "\e[0;31m[ERROR]\e[0m Not a valid option\n");
            break;
    }
}

SOCKET connectToServer() {

	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		printf("Connection error: %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
			ntohs(server.sin_port));

	// SEND and RECEIVE data
	printf("Sending message 1... \n");
	strcpy(sendBuff, "Hello, server.");
	send(s, sendBuff, sizeof(sendBuff), 0);

	printf("Receiving message 1... \n");
	recv(s, recvBuff, sizeof(recvBuff), 0);
	printf("Data received: %s \n", recvBuff);

	printf("Sending message 2... \n");
	strcpy(sendBuff, "Hello again.");
	send(s, sendBuff, sizeof(sendBuff), 0);
	printf("Data sent: %s \n", sendBuff);

	printf("Receiving message 2... \n");
	recv(s, recvBuff, sizeof(recvBuff), 0);
	printf("Data received: %s \n", recvBuff);

	printf("Sending last message... \n");
	strcpy(sendBuff, "Bye");
	send(s, sendBuff, sizeof(sendBuff), 0);
	printf("Data sent: %s \n", sendBuff);

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}