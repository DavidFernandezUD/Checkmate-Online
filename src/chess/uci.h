#include "bitboard.h"


#ifndef _UCI_H
#define _UCI_H


int parse_move(Position* pos, const char* move_string);

void parse_position(Position* pos, char* uci_command);


#endif // _UCI_H
