#include <stdbool.h>
#include "bitboard.h"


#ifndef _MOVEGEN_H
#define _MOVEGEN_H


// Attacks
// TODO: Define thsi function here in the header file and make it static inline
bool is_square_attacked(Position* position, Square square, Color color);

void print_attacked_squares(Position* Position);


// Move generator
void generate_moves(Position* position);


#endif // _MOVEGEN_H
