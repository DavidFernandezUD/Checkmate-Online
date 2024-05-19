#include "server_socket.h"
#include "server.h"

// Start the server (wait for connecting client)
void start_server() {
	WSADATA wsaData;
    SOCKET conn_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    char recvBuff[512];

	//cprintf("\nInitialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }

	//cSOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        WSACleanup();
        return;
    }

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
    printf("Incoming connection from: %s (%d)\n\n", inet_ntoa(client.sin_addr),
           ntohs(client.sin_port));

	// Close the listening socket (is not going to be used anymore)
    closesocket(conn_socket);

    // Initialize attack tables for chess engine
    init_piece_attacks();

    // Wait for "play" command from client
    int bytes_received = recv(comm_socket, recvBuff, sizeof(recvBuff) - 1, 0);
    if (bytes_received > 0) {
        recvBuff[bytes_received] = '\0'; // Null-terminate the buffer
        if (strcmp(recvBuff, "play") == 0) {
            // Start uci loop
            Position pos;
            uci_loop2(comm_socket, &pos);
        } else {
            printf("Unexpected command received: %s\n", recvBuff);
        }
    } else {
        printf("Failed to receive data from client.\n");
    }

	// CLOSE the socket and clean Winsock...
    closesocket(comm_socket);
    WSACleanup();
}
