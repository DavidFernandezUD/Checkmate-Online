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
    
    print_position(pos);

    print_attacked_squares(&pos);

    return 0;
}
