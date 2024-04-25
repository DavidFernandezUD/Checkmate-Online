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
#include "chess/perftest.h"
#include "chess/uci.h"


int main() {

    init_piece_attacks();

    Position pos;
    long nodes = 0;

    parse_fen(&pos, START_POSITION);
    print_position(pos);

    // int start = get_time_ms();
    // perf_test(&pos, 6, &nodes);
    // int end = get_time_ms();

    // printf("Time: %dms Nodes: %ld\n", end - start, nodes);
    
    int move = parse_move(&pos, "g1f3");

    if (move) {
        print_move(move);
    } else {
        printf("Ilegal move!\n");
    }

    return 0;
}
