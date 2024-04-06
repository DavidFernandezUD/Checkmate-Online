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

    init_piece_attacks();

    Position pos;
    long nodes = 0;

    parse_fen(&pos, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    // print_position(pos);

    int start = get_time_ms();
    perft_driver(&pos, 1, &nodes);
    int end = get_time_ms();

    printf("Time: %dms Nodes: %ld\n", end - start, nodes);

    return 0;
}
