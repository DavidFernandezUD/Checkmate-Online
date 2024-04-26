/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Universal Chess Interface (UCI) protocol implementation to allow the chess engine
 * to comunicate with general purpose chess user interfaces.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include "bitboard.h"


#ifndef _UCI_H
#define _UCI_H


int parse_move(Position* pos, const char* move_string);

void parse_position(Position* pos, const char* uci_command);

void parse_go(Position* pos, const char* uci_command);

void uci_loop(Position* pos);


#endif // _UCI_H
