#include <stdint.h>


#ifndef _BITBOARD_H
#define _BITBOARD_H


#define EMPTY_BOARD 0ULL

// Bitboard manipulation macros
#define GET_BIT(bitboard, index) ((bitboard) & (1ULL << (index)))
#define SET_BIT(bitboard, index) ((bitboard) |= (1ULL << (index)))
#define POP_BIT(bitboard, index) (GET_BIT((bitboard), (index)) ? bitboard ^= (1ULL << (index)) : 0)


typedef enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
} Square;

typedef enum {WHITE, BLACK} Color;

typedef enum {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING} Piece;


// Bitboard utilities
void print_bitboard(uint64_t bitboard);


// Leaper piece attacks
uint64_t get_pawn_attacks(Color color, Square square);

uint64_t get_knight_attacks(Square square);

uint64_t get_king_attacks(Square square);


// Sliding pieces attack masks
uint64_t get_bishop_attack_mask(Square square);

uint64_t get_bishop_attacks_blocked(Square square, uint64_t block);
   

uint64_t get_rook_attack_mask(Square square);
   
uint64_t get_rook_attacks_blocked(Square square, uint64_t block);


// Sliding piece ocupancies generator
uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask);


// Magic number generation
uint64_t find_magic_number(Square square, int relevant_bits, Piece piece);

void init_magic_numbers();


// Sliding piece attacks
void init_bishop_attacks();

void init_rook_attacks();

uint64_t get_bishop_attacks(int square, uint64_t occupancy);

uint64_t get_rook_attacks(int square, uint64_t occupancy);


// Attack table Initialization
void init_piece_attacks();


#endif // _BITBOARD_H
