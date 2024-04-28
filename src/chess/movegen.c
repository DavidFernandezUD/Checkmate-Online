/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Move generator that generates all posible legal moves from a given position and
 * adds them to a stack (MoveList).
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include <stdio.h>
#include <stdbool.h>
#include "bitboard.h"
#include "attack.h"
#include "move.h"


// TODO: Check if changing statement order improves engine performance
bool is_square_attacked(const Position* position, Square square, Color attacker_color) {

    if (attacker_color == WHITE) {
        if (pawn_attacks[BLACK][square] & position->bitboards[P]) return true;                              // White Pawns
        if (knight_attacks[square] & position->bitboards[N]) return true;                                   // White Knights
        if (get_bishop_attacks(square, position->occupancies[BOTH]) & position->bitboards[B]) return true;  // White Bishops
        if (get_rook_attacks(square, position->occupancies[BOTH]) & position->bitboards[R]) return true;    // White Rooks
        if (get_queen_attacks(square, position->occupancies[BOTH]) & position->bitboards[Q]) return true;   // White Queens
        if (king_attacks[square] & position->bitboards[K]) return true;                                     // White King
    } else {
        if (pawn_attacks[WHITE][square] & position->bitboards[p]) return true;                              // Black Pawns
        if (knight_attacks[square] & position->bitboards[n]) return true;                                   // Black Knights
        if (get_bishop_attacks(square, position->occupancies[BOTH]) & position->bitboards[b]) return true;  // Black Bishops
        if (get_rook_attacks(square, position->occupancies[BOTH]) & position->bitboards[r]) return true;    // Black Rooks
        if (get_queen_attacks(square, position->occupancies[BOTH]) & position->bitboards[q]) return true;   // Black Queens
        if (king_attacks[square] & position->bitboards[k]) return true;                                     // Black King
    }

    return false;
}


void print_attacked_squares(const Position* position) {

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            int square = row * 8 + col;

            if (col == 0) {
                printf(" %d  ", 8 - row);
            }

            printf(" %d", is_square_attacked(position, square, position->turn));
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");
}


// TODO: Refactor this function
static inline void generate_pawn_moves(const Position* position, MoveList* move_list) {

    int src_square;
    int dst_square;
    uint64_t bitboard;
    uint64_t attacks;

    // Pawn
    if (position->turn == WHITE) {
        bitboard = position->bitboards[P];
        while (bitboard) {

            src_square = get_ls1b_index(bitboard);

            // Quiet moves
            dst_square = src_square - 8;
            if ((dst_square >= a8) && !GET_BIT(position->occupancies[BOTH], dst_square)) {
                    
                // Pawn promotion
                if (dst_square <= h8) {
                    push_move(move_list, encode_move(src_square, dst_square, P, N, 0));
                    push_move(move_list, encode_move(src_square, dst_square, P, B, 0));
                    push_move(move_list, encode_move(src_square, dst_square, P, R, 0));
                    push_move(move_list, encode_move(src_square, dst_square, P, Q, 0));
                } else {

                    // 1 square moves
                    push_move(move_list, encode_move(src_square, dst_square, P, 0, 0));

                    // 2 square moves
                    dst_square = src_square - 16;
                    if (src_square >= a2 && src_square <= h2  && !GET_BIT(position->occupancies[BOTH], dst_square)) {
                        push_move(move_list, encode_move(src_square, dst_square, P, 0, DOUBLE_PUSH));
                    }
                }
            }

            // Captures
            attacks = pawn_attacks[WHITE][src_square] & position->occupancies[BLACK];
            while (attacks) {

                dst_square = get_ls1b_index(attacks);

                // Capture promotions
                if (dst_square <= h8) {
                    push_move(move_list, encode_move(src_square, dst_square, P, N, CAPTURE));
                    push_move(move_list, encode_move(src_square, dst_square, P, B, CAPTURE));
                    push_move(move_list, encode_move(src_square, dst_square, P, R, CAPTURE));
                    push_move(move_list, encode_move(src_square, dst_square, P, Q, CAPTURE));
                }
                // Normal capture
                else {
                    push_move(move_list, encode_move(src_square, dst_square, P, 0, CAPTURE));
                }
                POP_BIT(attacks, dst_square);
            }

            // Enpassant captures
            if (position->enpassant != NULL_SQUARE) {
                
                uint64_t enpassant_attacks = pawn_attacks[WHITE][src_square] & (1ULL << position->enpassant);

                if (enpassant_attacks) {
                    dst_square = get_ls1b_index(enpassant_attacks);
                    push_move(move_list, encode_move(src_square, dst_square, P, 0, CAPTURE | ENPASSANT));
                }
            }
            POP_BIT(bitboard, src_square);
        }
    } else {
        bitboard = position->bitboards[p];
        while (bitboard) {

            src_square = get_ls1b_index(bitboard);
            dst_square = src_square + 8;
                
            if ((dst_square <= h1) && !GET_BIT(position->occupancies[BOTH], dst_square)) {
                
                // Pawn promotion
                if (dst_square >= a1) {
                    push_move(move_list, encode_move(src_square, dst_square, p, n, 0));
                    push_move(move_list, encode_move(src_square, dst_square, p, b, 0));
                    push_move(move_list, encode_move(src_square, dst_square, p, r, 0));
                    push_move(move_list, encode_move(src_square, dst_square, p, q, 0));
                } else {
                    
                    // 1 square moves
                    push_move(move_list, encode_move(src_square, dst_square, p, 0, 0));
                    
                    // 2 square moves
                    dst_square = src_square + 16;
                    if (src_square >= a7 && src_square <= h7  && !GET_BIT(position->occupancies[BOTH], dst_square)) {
                        push_move(move_list, encode_move(src_square, dst_square, p, 0, DOUBLE_PUSH));
                    }
                }
            }

            // Captures
            attacks = pawn_attacks[BLACK][src_square] & position->occupancies[WHITE];
            while (attacks) {

                dst_square = get_ls1b_index(attacks);

                // Capture promotions
                if (dst_square >= a1) {
                    push_move(move_list, encode_move(src_square, dst_square, p, n, CAPTURE));
                    push_move(move_list, encode_move(src_square, dst_square, p, b, CAPTURE));
                    push_move(move_list, encode_move(src_square, dst_square, p, r, CAPTURE));
                    push_move(move_list, encode_move(src_square, dst_square, p, q, CAPTURE));
                }
                // Normal captures
                else {
                    push_move(move_list, encode_move(src_square, dst_square, p, 0, CAPTURE));
                }
                POP_BIT(attacks, dst_square);
            }

            // Enpassant captures
            if (position->enpassant != NULL_SQUARE) {

                uint64_t enpassant_attacks = pawn_attacks[BLACK][src_square] & (1ULL << position->enpassant);

                if (enpassant_attacks) {
                    dst_square = get_ls1b_index(enpassant_attacks);
                    push_move(move_list, encode_move(src_square, dst_square, p, 0, CAPTURE | ENPASSANT));
                }
            }
            POP_BIT(bitboard, src_square);
        }
    }
}


static inline void generate_castling_moves(const Position* position, MoveList* move_list) {

    if (position->turn == WHITE) {

        // Castling
        if (position->castling & WK) {
            if (!GET_BIT(position->occupancies[BOTH], f1) && !GET_BIT(position->occupancies[BOTH], g1)) {
                if (!is_square_attacked(position, e1, BLACK) & !is_square_attacked(position, f1, BLACK)) {
                    push_move(move_list, encode_move(e1, g1, K, 0, CASTLING));
                }
            }
        }
        if (position->castling & WQ) {
            if (!GET_BIT(position->occupancies[BOTH], b1) && !GET_BIT(position->occupancies[BOTH], c1) && !GET_BIT(position->occupancies[BOTH], d1)) {
                if (!is_square_attacked(position, d1, BLACK) & !is_square_attacked(position, e1, BLACK)) {
                    push_move(move_list, encode_move(e1, c1, K, 0, CASTLING));
                }
            }
        }
    } else {

        // Castling
        if (position->castling & BK) {
            if (!GET_BIT(position->occupancies[BOTH], f8) && !GET_BIT(position->occupancies[BOTH], g8)) {
                if (!is_square_attacked(position, e8, WHITE) & !is_square_attacked(position, f8, WHITE)) {
                    push_move(move_list, encode_move(e8, g8, k, 0, CASTLING));
                }
            }
        }
        if (position->castling & BQ) {
            if (!GET_BIT(position->occupancies[BOTH], b8) && !GET_BIT(position->occupancies[BOTH], c8) && !GET_BIT(position->occupancies[BOTH], d8)) {
                if (!is_square_attacked(position, d8, WHITE) & !is_square_attacked(position, e8, WHITE)) {
                    push_move(move_list, encode_move(e8, c8, k, 0, CASTLING));
                }
            }
        }
    }
}


void generate_moves(const Position* position, MoveList* move_list) {

    // Initialize move list
    move_list->top = 0;

    // NOTE: Pawn moves and castling moves are calculated separately
    generate_pawn_moves(position, move_list);
    generate_castling_moves(position, move_list);

    int start_piece = (position->turn == WHITE) ? N : n;
    int end_piece = (position->turn == WHITE) ? K : k;
    
    for (int piece = start_piece; piece <= end_piece; piece++) {

        int src_square;
        int dst_square;
        uint64_t bitboard;
        uint64_t attacks;

        bitboard = position->bitboards[piece];
        while (bitboard) {

            src_square = get_ls1b_index(bitboard);

            if (piece == N || piece == n) {
                attacks = knight_attacks[src_square] & ~position->occupancies[position->turn];
            } else if (piece == B || piece == b) {
                attacks = get_bishop_attacks(src_square, position->occupancies[BOTH]) & ~position->occupancies[position->turn];
            } else if (piece == R || piece == r) {
                attacks = get_rook_attacks(src_square, position->occupancies[BOTH]) & ~position->occupancies[position->turn];
            } else if (piece == Q || piece == q) {
                attacks = get_queen_attacks(src_square, position->occupancies[BOTH]) & ~position->occupancies[position->turn];
            } else if (piece == K || piece == k) {
                attacks = king_attacks[src_square] & ~position->occupancies[position->turn];
            }

            while (attacks) {
                
                dst_square = get_ls1b_index(attacks);

                // Quiet move
                if (!GET_BIT(position->occupancies[!position->turn], dst_square)) {
                    push_move(move_list, encode_move(src_square, dst_square, piece, 0, 0));
                } 
                // Capture
                else {
                    push_move(move_list, encode_move(src_square, dst_square, piece, 0, CAPTURE));
                }
                POP_BIT(attacks, dst_square);
            }
            POP_BIT(bitboard, src_square);
        }
    }
}
