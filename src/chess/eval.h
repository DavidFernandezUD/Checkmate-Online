/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Position evaluation function for move searching.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include "bitboard.h"
#include "move.h"


#ifndef _EVAL_H
#define _EVAL_H


extern const int material_score[12];

extern const int pawn_scores[64];
extern const int knight_scores[64];
extern const int bishop_scores[64];
extern const int rook_scores[64];
extern const int queen_scores[64];
extern const int king_scores[64];

extern const int mirror_squares[64];

extern const int mvv_lva[12][12];


static inline int evaluate(const Position* pos) {

    int score = 0;

    uint64_t bitboard;

    int square;

    for (int piece = P; piece <= k; piece++) {
        
        bitboard = pos->bitboards[piece];
        
        while (bitboard) {

            square = get_ls1b_index(bitboard);

            // Calculate static material score
            score += material_score[piece];

            // Calculate positional piece 
            switch (piece) {
                
                // White pieces
                case P: score += pawn_scores[square]; break;
                case N: score += knight_scores[square]; break;
                case B: score += bishop_scores[square]; break;
                case R: score += rook_scores[square]; break;
                case Q: score += queen_scores[square]; break;
                case K: score += king_scores[square]; break;

                // Black pieces
                case p: score -= pawn_scores[mirror_squares[square]]; break;
                case n: score -= knight_scores[mirror_squares[square]]; break;
                case b: score -= bishop_scores[mirror_squares[square]]; break;
                case r: score -= rook_scores[mirror_squares[square]]; break;
                case q: score -= queen_scores[mirror_squares[square]]; break;
                case k: score -= king_scores[mirror_squares[square]]; break;
            }

            POP_BIT(bitboard, square);
        }
    }

    // Changes score sign based on turn for min-max algorithm
    return (pos->turn == WHITE) ? score : -score;
}


static inline int eval_move(const Position* pos, int move) {

    if (IS_MOVE_CAPTURE(move)) {

        int attacking_piece = GET_MOVE_PIECE(move);

        // NOTE: If the destination square down't have a piece (enpassant), it will dfault to pawn
        int captured_piece = P;
        
        int dst_square = GET_MOVE_DST(move);

        int start_piece = (pos->turn == WHITE) ? p : P;
        int end_piece   = (pos->turn == WHITE) ? k : K;
        
        
        for (int piece = start_piece; piece <= end_piece; piece++) {
            if (GET_BIT(pos->bitboards[piece], dst_square)) {
                captured_piece = piece;
                break;
            }
        }

        return mvv_lva[attacking_piece][captured_piece];

    }

    return 0;
}


// TODO: Change insertion sort for a better sorting algorithm
static inline void sort_moves(const Position* pos, MoveList* list) {

    int move_scores[list->top];

    for (int i = 0; i < list->top; i++) {
        move_scores[i] = eval_move(pos, list->moves[i]);
    }

    for (int i = 0; i < list->top; i++) {
        for (int j = i + 1; j < list->top; j++) {
            if (move_scores[i] < move_scores[j]) {

                // Swap move scores
                int temp_score = move_scores[i];
                move_scores[i] = move_scores[j];
                move_scores[j] = temp_score;
            
                // Swap moves
                int temp_move = list->moves[i];
                list->moves[i] = list->moves[j];
                list->moves[j] = temp_move;
            }
        }
    }
}


#endif // _EVAL_H
