#include <stdbool.h>
#include "bitboard.h"


#ifndef _MOVEGEN_H
#define _MOVEGEN_H


bool is_square_attacked(Position* position, Square square);

void print_attacked_squares(Position* Position);


#endif // _MOVEGEN_H
