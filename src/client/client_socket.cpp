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

	 std::cout << "Connected to server: " << SERVER_IP << " (" << SERVER_PORT << ")" << std::endl << std::endl;

	return s;
}

// Send continuous moves to the server
void make_moves(SOCKET s) {
    char message[512];
    std::cout << "Enter a move to send to the server ('q' to quit):" << std::endl;
    while (true) {
        std::cin.getline(message, sizeof(message));
        if (strcmp(message, "q") == 0) {
            if (send(s, message, strlen(message), 0) == SOCKET_ERROR) {
                std::cerr << "Send failed with error code : " << WSAGetLastError() << std::endl;
                return;
            }
            std::cout << "Message sent: " << message << std::endl;
            break; // Salir del bucle después de enviar 'q'
        }
        if (send(s, message, strlen(message), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed with error code : " << WSAGetLastError() << std::endl;
            return;
        }
        std::cout << "Message sent: " << message << std::endl;

        // After sending a move, receive and display server's information
        receive_and_print_information(s);
    }
}

// Receive information from server and display it on the console
void receive_and_print_information(SOCKET s) {
    const int BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE];

    // Receive information from the server
    int bytes_received = recv(s, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        // Manejar errores de recepción o cierre de conexión
        if (bytes_received == 0) {
            std::cerr << "Connection closed by server" << std::endl;
        } else {
            std::cerr << "Error in recv()" << std::endl;
        }
        return;
    }

    // Display received information
    buffer[bytes_received] = '\0';
    std::cout << "Received data from server: " << std::endl << buffer << std::endl;
}

// Request user statistics from server
void request_user_statistics(SOCKET s, const char* username) {

}
