// CHESS INCLUDES
#include "../chess/bitboard.h"
#include "../chess/attack.h"
#include "../chess/random.h"
#include "../chess/movegen.h"
#include "../chess/move.h"
#include "../chess/makemove.h"
#include "../chess/perftest.h"
#include "../chess/uci.h"
#include "../chess/eval.h"
#include "../chess/search.h"
// CHESS INCLUDES

#include <stdio.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

#ifndef _SERVER_SOCKET_H
#define _SERVER_SOCKET_H

// Start the server (wait for connecting client)
void start_server();

#endif // _SERVER_SOCKET_H