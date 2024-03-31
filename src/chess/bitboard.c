#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "bitboard.h"
#include "attack.h"


static const char* square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};


// Piece repressentation
const Piece CHAR_TO_PIECE[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k
};

// Piece character representations
const char ASCII_PIECES[12] = "PNBRQKpnbrqk";
const char* UNICODE_PIECES[12] = {"♟︎", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔"};


void print_bitboard(uint64_t bitboard) {
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            
            // Index of current square in flattened array
            int square = row * 8 + col;
            
            // Print row index
            if (col == 0) {
                printf(" %d ", 8 - row);
            }

            // Print the value of the bit at the specified index in the 64 bit number
            printf(" %d", GET_BIT(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }

    // Print column character indexes
    printf("\n    a b c d e f g h\n\n");
    printf("Bitboard: %lu\n\n", bitboard);
}


void print_position(Position position) {
   
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            
            // Index of current square in flattened array
            int square = i * 8 + j;
            
            // Print row index
            if (j == 0) {
                printf(" %ld ", 8 - i);
            }
            
            // Loop over bitboards
            int piece = -1;
            for (int i = 0; i < 12; i++) {
                if (GET_BIT(position.bitboards[i], square)) {
                    piece = i;
                }
            }

            // For OS compatibility
            #ifdef WIN64
                printf(" %c", (piece == -1) ? '.' : ASCII_PIECES[piece]);
            #else
                printf(" %s", (piece == -1) ? "." : UNICODE_PIECES[piece]);
            #endif
        }
        printf("\n");
    }

    // Print column character indexes
    printf("\n    a b c d e f g h\n\n");

    printf("Turn:      %s\n", (position.turn == WHITE) ? "white" : "black");
    printf("Castling:  %c%c%c%c\n", (position.castling & WK) ? 'K' : '-',
                                      (position.castling & WQ) ? 'Q' : '-',
                                      (position.castling & BK) ? 'k' : '-',
                                      (position.castling & BQ) ? 'q' : '-'
    );
    printf("Enpassant: %s\n\n", (position.enpassant != null_square) ? square_to_coordinates[position.enpassant] : "no");
}


void parse_fen(Position* position, char* fen) {

    memset(position->bitboards, 0ULL, sizeof(position->bitboards));
    memset(position->occupancies, 0ULL, sizeof(position->occupancies));

    position->turn = WHITE;
    position->enpassant = null_square;
    position->castling = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; ) {

            int square = row * 8 + col;
            int character = *fen;

            if (isalpha(character)) {
                
                SET_BIT(position->bitboards[CHAR_TO_PIECE[character]], square);

                if (isupper(character)) {
                    SET_BIT(position->occupancies[WHITE], square);
                } else {
                    SET_BIT(position->occupancies[BLACK], square);
                }
                SET_BIT(position->occupancies[BOTH], square);

                col++;
                fen++;
            }

            if (isdigit(character)) {

                int num = character - '0';

                col += num;
                fen++;
            }

            if (character == '/') {
                fen++;
            }
        }
    }

    // Turn
    fen++;
    position->turn = (*fen == 'w') ? WHITE : BLACK;

    // Castling rights
    fen += 2;
    while (*fen != ' ') {

        switch (*fen) {
            case 'K': position->castling |= WK;
                break;
            case 'Q': position->castling |= WQ;
                break;
            case 'k': position->castling |= BK;
                break;
            case 'q': position->castling |= BQ;
                break;
        }

        fen++;
    }

    // En passant square
    fen++;
    if (*fen != '-') {
        int row = 8 - (fen[1] - '0');
        int col = fen[0] - 'a';

        position->enpassant = row * 8 + col;
    }
}


void init_position(Position* position) {
    parse_fen(position, START_POSITION);
}