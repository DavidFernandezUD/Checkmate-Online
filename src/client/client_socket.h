#include <iostream>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

#ifndef _CLIENT_SOCKET_H
#define _CLIENT_SOCKET_H

// Connect to socket (when "client.exe" is executed)
SOCKET connect_to_server();

// Send continuous moves to the server
void make_moves(SOCKET s);

// Receive information from server and display it on the console
void receive_and_print_information(SOCKET s);

#endif // _CLIENT_SOCKET_H