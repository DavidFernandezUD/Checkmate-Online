#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/attack.h"
#include "chess/random.h"
#include "chess/movegen.h"
#include "chess/move.h"
#include "chess/makemove.h"
#include "chess/perf.h"

int main() {

    // ==== PERF TEST ====
    init_piece_attacks();

    Position pos;
    long nodes = 0;

    parse_fen(&pos, START_POSITION);
    print_position(pos);

    int start = get_time_ms();
    perft_driver(&pos, 6, &nodes);
    int end = get_time_ms();

    printf("Time: %dms Nodes: %ld\n", end - start, nodes);

    return 0;
}
