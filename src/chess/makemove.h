#include "bitboard.h"
#include "move.h"


#ifndef _MAKEMOVE_H
#define _MAKEMOVE_H


typedef enum {ALL_MOVES, ATTACK_MOVES} MoveType;


static inline int make_move(Position* position, int move, MoveType type) {

    if (type == ALL_MOVES) {

        // Preserv board state befor move
        Position position_copy = *position;

        // Parse move
        int src_square = GET_MOVE_SRC(move);
        int dest_square = GET_MOVE_DEST(move);
        int piece = GET_MOVE_PIECE(move);
        int promotion = GET_MOVE_PROMOTION(move);
        int capture = IS_MOVE_CAPTURE(move);
        int double_push = IS_MOVE_DOUBLE(move);
        int enpassant = IS_MOVE_ENPASSANT(move);
        int castle = IS_MOVE_CASTLE(move);

        // Make move
        POP_BIT(position->bitboards[piece], src_square);
        SET_BIT(position->bitboards[piece], dest_square);

        // Captures
        if (capture) {

            int start_piece = (position->turn == WHITE) ? p : P;
            int end_piece = (position->turn == WHITE) ? k : K;
    
            for (int piece = start_piece; piece <= end_piece; piece++) {
                if (GET_BIT(position->bitboards[piece], dest_square)) {
                    POP_BIT(position->bitboards[piece], dest_square);
                    // TODO: Maybe pop accupancies bit also
                    break; // NOTE: No need to search for more pieces in a single square
                }
            }
        }

        // Promotions
        if (promotion) {
            POP_BIT(position->bitboards[piece], dest_square);
            SET_BIT(position->bitboards[promotion], dest_square);
        }

        // Enpassant
        if (enpassant) {
            if (position->turn == WHITE) {
                POP_BIT(position->bitboards[p], dest_square + 8);
            } else {
                POP_BIT(position->bitboards[P], dest_square - 8);
            }
        }

        // Reset empassant square
        position->enpassant = NULL_SQUARE;

        // Double pawn pushes
        if (double_push) {
            position->enpassant = (position->turn == WHITE) ? dest_square + 8 : dest_square - 8;
        }

        // FIXME: Remove this
        print_position(*position);
        *position = position_copy;

    } else {
        if (IS_MOVE_CAPTURE(move)) {
            make_move(position, move, ALL_MOVES);
        } else {
            return 0;
        }
    }
    return 1;
}


#endif // _MAKEMOVE_H