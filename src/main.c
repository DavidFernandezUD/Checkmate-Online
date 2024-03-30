#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/attack.h"
#include "chess/random.h"


int main() {

    Position pos = init_position();

    // SET_BIT(pos.bitboards[r], e4);
    // SET_BIT(pos.bitboards[P], f2);
    // SET_BIT(pos.bitboards[N], d6);

    // pos.turn = BLACK;
    // pos.enpassant = e4;
    // pos.castling = WK | WQ | BQ;

    parse_fen(&pos, START_POSITION);

    print_position(pos);

    return 0;
}

