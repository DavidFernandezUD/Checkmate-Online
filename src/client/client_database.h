#include <iostream>
#include <string>
#include <WinSock2.h>

#ifndef _CLIENT_DATABASE_H
#define _CLIENT_DATABASE_H

void send_username_to_server(SOCKET s, const std::string& data);

#endif // _CLIENT_DATABASE_H