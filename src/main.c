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

    // init_piece_attacks();

    // Position pos;
    // MoveList list;

    // parse_fen(&pos, "r3k2r/p1ppQpb1/bn2pnp1/2BPN2b/1p2P3/2N13p/PPPBqPPP/R3K2R w KQkq - 0 1 ");
    // print_position(pos);

    // generate_moves(&pos, &list);
    // print_move_list(&list);

    // for (int i = 0; i < list.top; i++) {

    //     // Preserve board state befor move
    //     Position position_copy = pos;

    //     int legal = make_move(&pos, list.moves[i], ALL_MOVES);
    //     print_move(list.moves[i]);
    //     printf("%s\n", (legal) ? "legal" : "ilegal");
    //     getchar();

    //     pos = position_copy;
    // }

    printf("Start!\n");
    int start = get_time_ms();
    getchar();
    int end = get_time_ms();

    printf("Time -> %dms\n", end - start);

    return 0;
}
