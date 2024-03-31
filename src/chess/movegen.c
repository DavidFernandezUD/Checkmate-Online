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
