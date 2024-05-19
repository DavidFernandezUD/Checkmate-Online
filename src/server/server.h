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
#include <string.h>
#include <stdlib.h>
#include "../../lib/sqlite/sqlite3.h"

#include "database.h"
#include "server_socket.h"

#define MAX_A_USERNAME_LEN 50
#define MAX_A_PASSWORD_LEN 50
#define MAX_A_CREDENTIALS_LENGTH 100

#define MAX_PARAMETER_LENGTH 10
#define MAX_VALUE_LENGTH 100

#define ERR_ILEGAL_MOVE 1

#ifndef _SERVER_H
#define _SERVER_H

class Game {

private:

    unsigned int n_move;
    char moves[2048];
    Position* pos;
    Color playerColor;

public:

    Game(Position& pos)

    Color getPlayerColor();

    unsigned int getMove();

    int makeMove(char* move);

    void makeEngineMove(Position* pos, int depth);
};

// Request admin credentials in the console
void requestCredentials(int* credentialsValid);

// Display main menu options
void show_main_menu();

// Handle user input in main menu
void handle_main_menu_option(sqlite3* db, char choice);

// Start uciloop (wait for an incoming move command)
void uci_loop2(SOCKET client_socket, Position* pos);

// Send chessboard status to client
void send_position_to_client(SOCKET client_socket, Position position);

#endif // _SERVER_H
