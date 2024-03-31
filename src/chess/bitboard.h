#include <stdint.h>


#ifndef _BITBOARD_H
#define _BITBOARD_H


// Bitboard manipulation macros
#define GET_BIT(bitboard, square) ((bitboard) & (1ULL << (square)))
#define SET_BIT(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define POP_BIT(bitboard, square) ((bitboard) &= ~(1ULL << (square)))


// Usefull FEN strings
#define EMPTY_BOARD "8/8/8/8/8/8/8/8 w - - "
#define START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define TRICKY_POSITION "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define KILLER_POSITION "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"


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

typedef enum {WHITE, BLACK, BOTH} Color;

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
    2 -> white + black occupancies
    */
    uint64_t occupancies[3];

    Color turn;
    Square enpassant;
    CastlingRights castling;

} Position;


// Utility functions
void print_bitboard(uint64_t bitboard);

void print_position(Position position);


// Position initialization and parsing
void parse_fen(Position* position, char* fen);

void init_position(Position* position);


#endif // _BITBOARD_H
