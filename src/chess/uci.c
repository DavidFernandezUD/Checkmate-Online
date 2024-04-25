#include <stdio.h>
#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "uci.h"


int parse_move(Position* pos, const char* move_string) {

    MoveList move_list;

    generate_moves(pos, &move_list);

    int src_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
    int dst_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

    for (int i = 0; i < move_list.top; i++) {
        
        int move = move_list.moves[i];

        if (src_square == GET_MOVE_SRC(move) && dst_square == GET_MOVE_DST(move)) {

            int promoted_piece = GET_MOVE_PROMOTION(move);
            if (promoted_piece) {
                if ((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q') {
                    return move;
                } else if ((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r') {
                    return move;
                } else if ((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b') {
                    return move;
                } else if ((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n') {
                    return move;
                }

                // If the promoted piece doesn't match with this move's one, skip to next move in move_list
                continue;
            }

            return move;
        }
    }

    return 0;
}
