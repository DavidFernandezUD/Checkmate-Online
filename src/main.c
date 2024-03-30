#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/attack.h"
#include "chess/random.h"


int main() {

    // init_piece_attacks();

    // uint64_t occupancy = 0ULL;

    // SET_BIT(occupancy, f7);
    // SET_BIT(occupancy, b5);
    // SET_BIT(occupancy, e8);
    // SET_BIT(occupancy, e4);

    // print_bitboard(get_bishop_attacks(d5, occupancy));

    // print_position();

    Position pos = {};

    SET_BIT(pos.bitboards[r], e4);
    SET_BIT(pos.bitboards[P], f2);
    SET_BIT(pos.bitboards[N], d6);

    pos.turn = BLACK;
    pos.enpassant = e4;
    pos.castling = WK | WQ | BQ;

    print_position(pos);

    return 0;
}

