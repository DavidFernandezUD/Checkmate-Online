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
    parse_fen(&pos, TRICKY_POSITION);
    pos.turn = BLACK;
    print_position(pos);

    generate_moves(&pos);

    return 0;
}
