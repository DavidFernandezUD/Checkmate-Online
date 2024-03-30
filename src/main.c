#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/attack.h"
#include "chess/random.h"


int main() {

    // Position pos;

    // init_position(&pos);

    // parse_fen(&pos, TRICKY_POSITION);

    // print_position(pos);

    init_piece_attacks();

    uint64_t bb = EMPTY_BITBOARD;

    SET_BIT(bb, e7);
    SET_BIT(bb, e3);
    SET_BIT(bb, g4);
    SET_BIT(bb, c5);
    SET_BIT(bb, b7);
    SET_BIT(bb, g6);
    SET_BIT(bb, h1);
    SET_BIT(bb, d3);

    print_bitboard(bb);

    print_bitboard(get_queen_attacks(e4, bb));

    return 0;
}

