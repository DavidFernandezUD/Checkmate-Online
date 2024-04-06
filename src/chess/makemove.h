#include "bitboard.h"
#include "move.h"


#ifndef _MAKEMOVE_H
#define _MAKEMOVE_H


typedef enum {ALL_MOVES, CAPTURE_MOVES} MoveType;

static const int CASTLING_RIGHTS[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};


static inline int make_move(Position* position, int move, MoveType type) {

    if (type == ALL_MOVES) {

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

        // Updating occupancies
        POP_BIT(position->occupancies[position->turn], src_square);
        POP_BIT(position->occupancies[BOTH], src_square);
        SET_BIT(position->occupancies[position->turn], dest_square);
        SET_BIT(position->occupancies[BOTH], dest_square);

        // Captures
        if (capture) {

            int start_piece = (position->turn == WHITE) ? p : P;
            int end_piece = (position->turn == WHITE) ? k : K;
    
            for (int piece = start_piece; piece <= end_piece; piece++) {
                if (GET_BIT(position->bitboards[piece], dest_square)) {
                    POP_BIT(position->bitboards[piece], dest_square);
                    
                    // Update opposite side occupancy
                    POP_BIT(position->occupancies[!position->turn], dest_square);
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
                POP_BIT(position->occupancies[BLACK], dest_square + 8);
                POP_BIT(position->occupancies[BOTH], dest_square + 8);
            } else {
                POP_BIT(position->bitboards[P], dest_square - 8);
                POP_BIT(position->occupancies[WHITE], dest_square - 8);
                POP_BIT(position->occupancies[BOTH], dest_square - 8);
            }
        }

        // Reset empassant square
        position->enpassant = NULL_SQUARE;

        // Double pawn pushes
        if (double_push) {
            position->enpassant = (position->turn == WHITE) ? dest_square + 8 : dest_square - 8;
        }

        // Castling
        if (castle) {
            switch (dest_square) {
                case g1:
                    POP_BIT(position->bitboards[R], h1);
                    SET_BIT(position->bitboards[R], f1);
                    POP_BIT(position->occupancies[WHITE], h1);
                    POP_BIT(position->occupancies[BOTH], h1);
                    SET_BIT(position->occupancies[WHITE], f1);
                    SET_BIT(position->occupancies[BOTH], f1);
                    break;
                case c1:
                    POP_BIT(position->bitboards[R], a1);
                    SET_BIT(position->bitboards[R], d1);
                    POP_BIT(position->occupancies[WHITE], a1);
                    POP_BIT(position->occupancies[BOTH], a1);
                    SET_BIT(position->occupancies[WHITE], d1);
                    SET_BIT(position->occupancies[BOTH], d1);
                    break;
                case g8:
                    POP_BIT(position->bitboards[r], h8);
                    SET_BIT(position->bitboards[r], f8);
                    POP_BIT(position->occupancies[BLACK], h8);
                    POP_BIT(position->occupancies[BOTH], h8);
                    SET_BIT(position->occupancies[BLACK], f8);
                    SET_BIT(position->occupancies[BOTH], f8);
                    break;
                case c8:
                    POP_BIT(position->bitboards[r], a8);
                    SET_BIT(position->bitboards[r], d8);
                    POP_BIT(position->occupancies[BLACK], a8);
                    POP_BIT(position->occupancies[BOTH], a8);
                    SET_BIT(position->occupancies[BLACK], d8);
                    SET_BIT(position->occupancies[BOTH], d8);
                    break;
            }
        }

        // Update castling rights
        position->castling &= CASTLING_RIGHTS[src_square];  // Check for king or rooks moving from initial position
        position->castling &= CASTLING_RIGHTS[dest_square]; // Check for pieces capturing rooks at initial position

        // Make sure king is not in check after making move
        if (position->turn == WHITE) {
            if (is_square_attacked(position, get_ls1b_index(position->bitboards[K]), BLACK)) return 0;
        } else {
            if (is_square_attacked(position, get_ls1b_index(position->bitboards[k]), WHITE)) return 0;
        }

        // // Change turn
        position->turn ^= 1;
        
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