/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Search function to find the best move from a tree of possible moves using
 * mini-max algorithms and alpha-beta pruning.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include <stdio.h>
#include "bitboard.h"
#include "search.h"


void search_position(Position* pos, int depth) {

    int best_move;
    int half_moves = 0;
    int nodes;

    int score = negmax(pos, depth, -99999, 99999, &half_moves, &nodes, &best_move);

    printf("bestmove %s%s\n", square_to_coordinates[GET_MOVE_SRC(best_move)], square_to_coordinates[GET_MOVE_DST(best_move)]);
}
