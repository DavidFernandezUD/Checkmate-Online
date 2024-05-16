#include "client_socket.h"

// Connect to socket (when "client.exe" is executed)
SOCKET connect_to_server() {
	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock. Error Code: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

	// CREATE SOCKET
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Failed to create socket. Error Code: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

	// CONNECT to remote server
	 if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server. Error Code: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        return INVALID_SOCKET;
    }

	 std::cout << "Connected to server: " << SERVER_IP << " (" << SERVER_PORT << ")" << std::endl;

	return s;
}

// Send continuous messages to the server
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