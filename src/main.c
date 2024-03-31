#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/attack.h"
#include "chess/random.h"
#include "chess/movegen.h"


int main() {

    init_piece_attacks();

    Position pos;
    parse_fen(&pos, "r2nkn1r/pR1pqpR1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBRPrP/R3K2R w KQkq - 0 1 ");
    pos.turn = BLACK;
    print_position(pos);

    generate_moves(&pos);

    return 0;
}
