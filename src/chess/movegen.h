/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Move generator that generates all posible legal moves from a given position and
 * adds them to a stack (MoveList).
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include <stdbool.h>
#include "bitboard.h"
#include "move.h"


#ifndef _MOVEGEN_H
#define _MOVEGEN_H


// Attacks
bool is_square_attacked(Position* position, Square square, Color attacker_color);

void print_attacked_squares(Position* Position);


// Move generator
void generate_moves(Position* position, MoveList* move_list);


#endif // _MOVEGEN_H
