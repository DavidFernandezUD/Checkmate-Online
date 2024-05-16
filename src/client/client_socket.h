#include <iostream>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

#ifndef _CLIENT_SOCKET_H
#define _CLIENT_SOCKET_H

// Connect to socket (when "client.exe" is executed)
SOCKET connect_to_server();

// Send continuous messages to the server
void send_messages(SOCKET s);

#endif // _CLIENT_SOCKET_H