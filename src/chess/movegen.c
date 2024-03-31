#include <stdio.h>
#include <stdbool.h>
#include "bitboard.h"
#include "attack.h"


// TODO: Check if changing statement order improves engine performance
bool is_square_attacked(Position* position, Square square) {

    if (position->turn == WHITE) {
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
        if (get_queen_attacks(square, position->occupancies[BOTH]) & position->bitboards[Q]) return true;   // Black Queens
        if (king_attacks[square] & position->bitboards[k]) return true;                                     // Black King
    }

    return false;
}


void print_attacked_squares(Position* position) {

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {

            int square = row * 8 + col;

            if (col == 0) {
                printf(" %d  ", 8 - row);
            }

            printf(" %d", is_square_attacked(position, square));
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");
}


// TODO: Split each piece move generation into separate static inline functions
void generate_moves(Position* position) {
    
    int src_square, dest_square;
    uint64_t bitboard, attacks;

    for (int piece = P; piece <= k; piece++) {
        
        bitboard = position->bitboards[piece];

        // Pawn and kastling moves
        if (position->turn == WHITE) {
            if (piece == P) {
                while (bitboard) {

                    src_square = get_ls1b_index(bitboard);

                    dest_square = src_square - 8;
                    
                    if ((dest_square >= a8) && !GET_BIT(position->occupancies[BOTH], dest_square)) {
                        
                        // Pawn promotion
                        if (dest_square <= h8) {
                            printf("Promotion N: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                            printf("Promotion B: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                            printf("Promotion R: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                            printf("Promotion Q: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                        } else {
                            // 2 square moves
                            if (src_square >= a2 && src_square <= h2  && !GET_BIT(position->occupancies[BOTH], src_square - 16)) {
                                printf("2 square: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square - 8]);
                            }

                            // 1 square moves
                            printf("1 square: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                        }
                    }

                    POP_BIT(bitboard, src_square);
                }
            } else if (piece == K) {

            }
        } else {
                if (piece == p) {
                while (bitboard) {

                    src_square = get_ls1b_index(bitboard);

                    dest_square = src_square + 8;
                    
                    if ((dest_square <= h1) && !GET_BIT(position->occupancies[BOTH], dest_square)) {
                        
                        // Pawn promotion
                        if (dest_square >= a1) {
                            printf("Promotion N: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                            printf("Promotion B: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                            printf("Promotion R: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                            printf("Promotion Q: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                        } else {
                            // 2 square moves
                            if (src_square >= a7 && src_square <= h7  && !GET_BIT(position->occupancies[BOTH], src_square + 16)) {
                                printf("2 square: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square + 8]);
                            }

                            // 1 square moves
                            printf("1 square: %s%s\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                        }
                    }

                    POP_BIT(bitboard, src_square);
                }
            } else if (piece == k) {

            }
        }

        // Knight moves

        // Bishop moves

        // Rook moves

        // Queen moves

        // King moves
    }
}
