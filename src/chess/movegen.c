#include <stdio.h>
#include <stdbool.h>
#include "bitboard.h"
#include "attack.h"


// TODO: Check if changing statement order improves engine performance
bool is_square_attacked(Position* position, Square square, Color color) {

    if (color == WHITE) {
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

            printf(" %d", is_square_attacked(position, square, position->turn));
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");
}


// TODO: Refactor this function
static inline void generate_pawn_moves(Position* position) {

    int src_square;
    int dest_square;
    uint64_t bitboard;
    uint64_t attacks;

    // Pawn
    if (position->turn == WHITE) {
        bitboard = position->bitboards[P];
        while (bitboard) {

            src_square = get_ls1b_index(bitboard);

            // Quiet moves
            dest_square = src_square - 8;
            if ((dest_square >= a8) && !GET_BIT(position->occupancies[BOTH], dest_square)) {
                    
                // Pawn promotion
                if (dest_square <= h8) {
                    printf("%s%s Pawn promotion N\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn promotion B\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn promotion R\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn promotion Q\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                } else {

                    // 1 square moves
                    printf("%s%s Pawn push\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);

                    // 2 square moves
                    dest_square = src_square - 16;
                    if (src_square >= a2 && src_square <= h2  && !GET_BIT(position->occupancies[BOTH], dest_square)) {
                            printf("%s%s Pawn double push\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    }
                }
            }

            // Captures
            attacks = pawn_attacks[WHITE][src_square] & position->occupancies[BLACK];
            while (attacks) {

                dest_square = get_ls1b_index(attacks);

                // Capture promotions
                if (dest_square <= h8) {
                    printf("%s%s Pawn capture promotion N\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn capture promotion B\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn capture promotion R\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn capture promotion Q\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                }
                // Normal promotions
                else {
                    printf("%s%s Pawn capture\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                }
                POP_BIT(attacks, dest_square);
            }

            // Enpassant captures
            if (position->enpassant != NULL_SQUARE) {
                
                uint64_t enpassant_attacks = pawn_attacks[WHITE][src_square] & (1ULL << position->enpassant);

                if (enpassant_attacks) {
                    dest_square = get_ls1b_index(enpassant_attacks);
                    printf("%s%s Pawn enpassant capture\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                }
            }
            POP_BIT(bitboard, src_square);
        }
    } else {
        bitboard = position->bitboards[p];
        while (bitboard) {

            src_square = get_ls1b_index(bitboard);
            dest_square = src_square + 8;
                
            if ((dest_square <= h1) && !GET_BIT(position->occupancies[BOTH], dest_square)) {
                
                // Pawn promotion
                if (dest_square >= a1) {
                    printf("%s%s Pawn promotion N\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn promotion B\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn promotion R\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn promotion Q\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                } else {
                    
                    // 1 square moves
                    printf("%s%s Pawn push\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    
                    // 2 square moves
                    dest_square = src_square + 16;
                    if (src_square >= a7 && src_square <= h7  && !GET_BIT(position->occupancies[BOTH], dest_square)) {
                        printf("%s%s Pawn double push\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    }
                }
            }

            // Captures
            attacks = pawn_attacks[BLACK][src_square] & position->occupancies[WHITE];
            while (attacks) {

                dest_square = get_ls1b_index(attacks);

                // Capture promotions
                if (dest_square >= a1) {
                    printf("%s%s Pawn capture promotion N\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn capture promotion B\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn capture promotion R\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                    printf("%s%s Pawn capture promotion Q\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                }
                // Normal captures
                else {
                    printf("%s%s Pawn capture\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                }
                POP_BIT(attacks, dest_square);
            }

            // Enpassant captures
            if (position->enpassant != NULL_SQUARE) {

                uint64_t enpassant_attacks = pawn_attacks[BLACK][src_square] & (1ULL << position->enpassant);

                if (enpassant_attacks) {
                    dest_square = get_ls1b_index(enpassant_attacks);
                    printf("%s%s Pawn enpassant capture\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
                }
            }
            POP_BIT(bitboard, src_square);
        }
    }
}


static inline void generate_king_moves(Position* position) {

    if (position->turn == WHITE) {

        // Castling
        if (position->castling & WK) {
            if (!GET_BIT(position->occupancies[BOTH], f1) && !GET_BIT(position->occupancies[BOTH], g1)) {
                if (!is_square_attacked(position, e1, BLACK) & !is_square_attacked(position, f1, BLACK)) {
                    printf("e1g1 Castling\n");
                }
            }
        }
        if (position->castling & WQ) {
            if (!GET_BIT(position->occupancies[BOTH], b1) && !GET_BIT(position->occupancies[BOTH], c1) && !GET_BIT(position->occupancies[BOTH], d1)) {
                if (!is_square_attacked(position, d1, BLACK) & !is_square_attacked(position, e1, BLACK)) {
                    printf("e1c1 Castling\n");
                }
            }
        }

        // Attacks
        // TODO: Inplement this

    } else {

        // Castling
        if (position->castling & BK) {
            if (!GET_BIT(position->occupancies[BOTH], f8) && !GET_BIT(position->occupancies[BOTH], g8)) {
                if (!is_square_attacked(position, e8, WHITE) & !is_square_attacked(position, f8, WHITE)) {
                    printf("e8g8 Castling\n");
                }
            }
        }
        if (position->castling & BQ) {
            if (!GET_BIT(position->occupancies[BOTH], b8) && !GET_BIT(position->occupancies[BOTH], c8) && !GET_BIT(position->occupancies[BOTH], d8)) {
                if (!is_square_attacked(position, d8, WHITE) & !is_square_attacked(position, e8, WHITE)) {
                    printf("e8c8 Castling\n");
                }
            }
        }
    }
}


// TODO: Refactor normal move generation inside generate_moves() to avoid code repetition
static inline void generate_knight_moves(Position* position) {

    int src_square;
    int dest_square;
    uint64_t bitboard;
    uint64_t attacks;

    bitboard = (position->turn == WHITE) ? position->bitboards[N] : position->bitboards[n];
    while (bitboard) {

        src_square = get_ls1b_index(bitboard);

        attacks = knight_attacks[src_square] & ~position->occupancies[position->turn];

        while (attacks) {
            
            dest_square = get_ls1b_index(attacks);

            // Quiet move
            if (!GET_BIT(position->occupancies[!position->turn], dest_square)) {
                printf("%s%s Knight move\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
            } 
            // Capture
            else {
                printf("%s%s Knight capture\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
            }
            POP_BIT(attacks, dest_square);
        }
        POP_BIT(bitboard, src_square);
    }
}


static inline void generate_bishop_moves(Position* position) {

    int src_square;
    int dest_square;
    uint64_t bitboard;
    uint64_t attacks;

    bitboard = (position->turn == WHITE) ? position->bitboards[B] : position->bitboards[b];
    while (bitboard) {

        src_square = get_ls1b_index(bitboard);

        attacks = get_bishop_attacks(src_square, position->occupancies[BOTH]) & ~position->occupancies[position->turn];

        while (attacks) {
            
            dest_square = get_ls1b_index(attacks);

            // Quiet move
            if (!GET_BIT(position->occupancies[!position->turn], dest_square)) {
                printf("%s%s Bishop move\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
            } 
            // Capture
            else {
                printf("%s%s Bishop capture\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
            }
            POP_BIT(attacks, dest_square);
        }
        POP_BIT(bitboard, src_square);
    }
}


static inline void generate_rook_moves(Position* position) {

    int src_square;
    int dest_square;
    uint64_t bitboard;
    uint64_t attacks;

    bitboard = (position->turn == WHITE) ? position->bitboards[R] : position->bitboards[r];
    while (bitboard) {

        src_square = get_ls1b_index(bitboard);

        attacks = get_rook_attacks(src_square, position->occupancies[BOTH]) & ~position->occupancies[position->turn];

        while (attacks) {
            
            dest_square = get_ls1b_index(attacks);

            // Quiet move
            if (!GET_BIT(position->occupancies[!position->turn], dest_square)) {
                printf("%s%s Rook move\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
            } 
            // Capture
            else {
                printf("%s%s Rook capture\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
            }
            POP_BIT(attacks, dest_square);
        }
        POP_BIT(bitboard, src_square);
    }
}


static inline void generate_queen_moves(Position* position) {

    int src_square;
    int dest_square;
    uint64_t bitboard;
    uint64_t attacks;

    bitboard = (position->turn == WHITE) ? position->bitboards[Q] : position->bitboards[q];
    while (bitboard) {

        src_square = get_ls1b_index(bitboard);

        attacks = get_queen_attacks(src_square, position->occupancies[BOTH]) & ~position->occupancies[position->turn];

        while (attacks) {
            
            dest_square = get_ls1b_index(attacks);

            // Quiet move
            if (!GET_BIT(position->occupancies[!position->turn], dest_square)) {
                printf("%s%s Queen move\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
            } 
            // Capture
            else {
                printf("%s%s Queen capture\n", square_to_coordinates[src_square], square_to_coordinates[dest_square]);
            }
            POP_BIT(attacks, dest_square);
        }
        POP_BIT(bitboard, src_square);
    }
}


void generate_moves(Position* position) {

    // Pawn moves
    // generate_pawn_moves(position);

    // Knight moves
    // generate_knight_moves(position);

    // Bishop moves
    // generate_bishop_moves(position);

    // Rook moves
    // generate_rook_moves(position);

    // Queen moves
    // generate_queen_moves(position);

    // King moves
    // generate_king_moves(position);
}
