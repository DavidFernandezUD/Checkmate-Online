#include <iostream>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

#ifndef _CLIENT_SOCKET_H
#define _CLIENT_SOCKET_H

SOCKET connect_to_server();

void send_messages(SOCKET s);

#endif // _CLIENT_SOCKET_H