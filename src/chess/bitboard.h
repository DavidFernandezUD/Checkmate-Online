#include <stdint.h>


#ifndef _BITBOARD_H
#define _BITBOARD_H


// Bitboard manipulation macros
#define GET_BIT(bitboard, square) ((bitboard) & (1ULL << (square)))
#define SET_BIT(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define POP_BIT(bitboard, square) ((bitboard) &= ~(1ULL << (square)))


// enum utils
typedef enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    null_square
} Square;

typedef enum {WHITE, BLACK} Color;

typedef enum {P, N, B, R, Q, K, p, n, b, r, q, k} Piece;

typedef enum {WK=1, WQ=2, BK=4, BQ=8} CastlingRights;


// Position representation
typedef struct {

    /* ==== PIECE BITBOARDS ====
    0  -> white pawns
    1  -> white knights
    2  -> wihte bishops
    3  -> white rooks
    4  -> white queens
    5  -> white king
    6  -> black pawns
    7  -> black knights
    8  -> black bishops
    9  -> black rooks
    10 -> black queens
    11 -> black king
    */
    uint64_t bitboards[12];

    /* ==== Occupancy bitboards ==== 
    0 -> white occupancies
    1 -> black occupancies
    */
    uint64_t occupancies[2];

    Color turn;
    Square enpassant;
    CastlingRights castling;

} Position;


// Utility functions
void print_bitboard(uint64_t bitboard);

void print_position(Position position);


// Position creation and initialization
Position init_position();


#endif // _BITBOARD_H
