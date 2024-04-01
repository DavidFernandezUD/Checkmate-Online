#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/attack.h"
#include "chess/random.h"
#include "chess/movegen.h"
#include "chess/move.h"


int main() {

    init_piece_attacks();

    Position pos;
    MoveList list;

    parse_fen(&pos, TRICKY_POSITION);
    print_position(pos);

    generate_moves(&pos, &list);

    print_move_list(&list);

    return 0;
}
