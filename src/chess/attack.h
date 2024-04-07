#include <stdint.h>
#include "bitboard.h"


#ifndef _ATTACK_H
#define _ATTACK_H


// Usefull bitboard masks
#define EMPTY_BITBOARD 0ULL
#define NOT_A_COL 18374403900871474942ULL
#define NOT_AB_COL 18229723555195321596ULL
#define NOT_H_COL 9187201950435737471ULL
#define NOT_GH_COL 4557430888798830399ULL


typedef enum {ROOK, BISHOP} IsBishop;


// Piece attack tables
extern uint64_t pawn_attacks[2][64];
extern uint64_t knight_attacks[64];
extern uint64_t king_attacks[64];
extern uint64_t bishop_attacks[64][512];
extern uint64_t rook_attacks[64][4096];

// Attack masks
extern uint64_t bishop_attack_masks[64];
extern uint64_t rook_attack_masks[64];

// Magic numbers
extern uint64_t bishop_magic_numbers[64];
extern uint64_t rook_magic_numbers[64];

// Relevant bits
extern const int bishop_relevant_bits[64];
extern const int rook_relevant_bits[64];


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
uint64_t find_magic_number(Square square, int relevant_bits, IsBishop piece);

void init_magic_numbers();


// Sliding piece attacks
void init_bishop_attacks();

void init_rook_attacks();

static inline uint64_t get_bishop_attacks(int square, uint64_t occupancy) {

    occupancy &= bishop_attack_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    return bishop_attacks[square][occupancy];
}

static inline uint64_t get_rook_attacks(int square, uint64_t occupancy) {

    occupancy &= rook_attack_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];
    
    return rook_attacks[square][occupancy];
}

static inline uint64_t get_queen_attacks(int square, uint64_t occupancy) {

    uint64_t bishop_occupancy = occupancy;
    uint64_t rook_occupancy = occupancy;

    bishop_occupancy &= bishop_attack_masks[square];
    bishop_occupancy *= bishop_magic_numbers[square];
    bishop_occupancy >>= 64 - bishop_relevant_bits[square];

    rook_occupancy &= rook_attack_masks[square];
    rook_occupancy *= rook_magic_numbers[square];
    rook_occupancy >>= 64 - rook_relevant_bits[square];
    
    return bishop_attacks[square][bishop_occupancy] | rook_attacks[square][rook_occupancy];
}


// Attack table Initialization
void init_piece_attacks();


#endif // _ATTACK_H
