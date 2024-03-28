#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "bitboard.h"
#include "random.h"


// Usefull bitboard masks
const uint64_t NOT_A_COL = 18374403900871474942ULL;
const uint64_t NOT_AB_COL = 18229723555195321596ULL;
const uint64_t NOT_H_COL = 9187201950435737471ULL;
const uint64_t NOT_GH_COL = 4557430888798830399ULL;


// Precalculated attack tables
uint64_t pawn_attacks[2][64];
uint64_t knight_attacks[64];
uint64_t king_attacks[64];


// Maximum number of squares a rook or bishop in each square is able to accupy
const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const int rook__relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};


// Bitboard index utilities
const char* square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};


// TODO: Search in CPW for more efficient implementation (bitcount)
static inline int count_bits(uint64_t bitboard) {
    
    int count = 0;

    while (bitboard) {
        count++;
        bitboard &= bitboard - 1;
    }

    return count;
}


// TODO: Search in CPW for more efficient implementation (bitscan)
static inline int get_ls1b_index(uint64_t bitboard) {
    
    if (bitboard) {

        // Creates a sequence of trailing ones until the less significant bit, and count it
        return count_bits((bitboard & -bitboard) - 1);
    }
    
    return -1;
}


void print_bitboard(uint64_t bitboard) {
    
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            
            // Index of current square in flattened array
            int index = i * 8 + j;
            
            // Print row index
            if (j == 0) {
                printf(" %ld ", 8 - i);
            }

            // Print the value of the bit at the specified index in the 64 bit number
            printf(" %d", GET_BIT(bitboard, index) ? 1 : 0);
        }
        printf("\n");
    }

    // Print column character indexes
    printf("\n    a b c d e f g h\n\n");
    printf("Bitboard: %lu\n\n", bitboard);
}



// Pawn attacks
uint64_t get_pawn_attacks(Color color, Square square) {

    uint64_t bitboard = EMPTY_BOARD;
    SET_BIT(bitboard, square);

    uint64_t attacks = EMPTY_BOARD;
    
    if (color == WHITE) {
        if ((bitboard >> 7) & NOT_A_COL) {
            attacks |= (bitboard >> 7);
        }

        if ((bitboard >> 9) & NOT_H_COL) {
            attacks |= (bitboard >> 9);
        }
    } else /*BLACK*/ {
        if ((bitboard << 7) & NOT_H_COL) {
            attacks |= (bitboard << 7);
        }

        if ((bitboard << 9) & NOT_A_COL) {
            attacks |= (bitboard << 9);
        }
    }

    return attacks;
}


// Knight attacks
uint64_t get_knight_attacks(Square square) {
    
    uint64_t bitboard = EMPTY_BOARD;
    SET_BIT(bitboard, square);

    uint64_t attacks = EMPTY_BOARD;
    
    if (bitboard & NOT_A_COL) {
        attacks |= (bitboard >> 17);
        attacks |= (bitboard << 15);
    }

    if (bitboard & NOT_H_COL) {
        attacks |= (bitboard >> 15);
        attacks |= (bitboard << 17);
    }

    if (bitboard & NOT_AB_COL) {
        attacks |= (bitboard >> 10);
        attacks |= (bitboard << 6);
    }

    if (bitboard & NOT_GH_COL) {
        attacks |= (bitboard >> 6);
        attacks |= (bitboard << 10);
    }

    return attacks;
}


// King attacks
uint64_t get_king_attacks(Square square) {
     
    uint64_t bitboard = EMPTY_BOARD;
    SET_BIT(bitboard, square);

    uint64_t attacks = EMPTY_BOARD;
    
    if (bitboard & NOT_A_COL) {
        attacks |= (bitboard >> 9);
        attacks |= (bitboard >> 1);
        attacks |= (bitboard << 7);
    }

    if (bitboard & NOT_H_COL) {
        attacks |= (bitboard >> 7);
        attacks |= (bitboard << 1);
        attacks |= (bitboard << 9);
    }

    attacks |= (bitboard >> 8);
    attacks |= (bitboard << 8);

    return attacks;   
}


// Bishop attacks
uint64_t get_bishop_attacks(Square square) {
    
    uint64_t attacks = EMPTY_BOARD;
    
    int row = square / 8;
    int col = square % 8;

    int i, j;
    for (i = row + 1, j = col + 1; i <= 6 && j <= 6; i++, j++) {
        SET_BIT(attacks, (i * 8 + j));
    }
    for (i = row - 1, j = col + 1; i >= 1 && j <= 6; i--, j++) {
        SET_BIT(attacks, (i * 8 + j));
    }
    for (i = row + 1, j = col - 1; i <= 6 && j >= 1; i++, j--) {
        SET_BIT(attacks, (i * 8 + j));
    }
    for (i = row - 1, j = col - 1; i >= 1 && j >= 1; i--, j--) {
        SET_BIT(attacks, (i * 8 + j));
    }

    return attacks;
}


uint64_t get_bishop_attacks_blocked(Square square, uint64_t block) {
    
    uint64_t attacks = EMPTY_BOARD;
    
    int row = square / 8;
    int col = square % 8;

    int i, j;
    for (i = row + 1, j = col + 1; i <= 7 && j <= 7; i++, j++) {
        SET_BIT(attacks, (i * 8 + j));
        if (1ULL << (i * 8 + j) & block) break;
    }
    for (i = row - 1, j = col + 1; i >= 0 && j <= 7; i--, j++) {
        SET_BIT(attacks, (i * 8 + j));
        if (1ULL << (i * 8 + j) & block) break;
    }
    for (i = row + 1, j = col - 1; i <= 7 && j >= 0; i++, j--) {
        SET_BIT(attacks, (i * 8 + j));
        if (1ULL << (i * 8 + j) & block) break;
    }
    for (i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        SET_BIT(attacks, (i * 8 + j));
        if (1ULL << (i * 8 + j) & block) break;
    }

    return attacks;
}


// Rook attacks
uint64_t get_rook_attacks(Square square) {
    
    uint64_t attacks = EMPTY_BOARD;
    
    int row = square / 8;
    int col = square % 8;
    
    for (int i = row + 1; i <= 6; i++) {
        SET_BIT(attacks, (i * 8 + col));
    }
    for (int i = row - 1; i >= 1; i--) {
        SET_BIT(attacks, (i * 8 + col));
    }
    for (int i = col + 1; i <= 6; i++) {
        SET_BIT(attacks, (row * 8 + i));
    }
    for (int i = col - 1; i >= 1; i--) {
        SET_BIT(attacks, (row * 8 + i));
    }   

    return attacks;
}


uint64_t get_rook_attacks_blocked(Square square, uint64_t block) {
    
    uint64_t attacks = EMPTY_BOARD;
    
    int row = square / 8;
    int col = square % 8;
    
    for (int i = row + 1; i <= 7; i++) {
        SET_BIT(attacks, (i * 8 + col));
        if (1ULL << (i * 8 + col) & block) break;
    }
    for (int i = row - 1; i >= 0; i--) {
        SET_BIT(attacks, (i * 8 + col));
        if (1ULL << (i * 8 + col) & block) break;
    }
    for (int i = col + 1; i <= 7; i++) {
        SET_BIT(attacks, (row * 8 + i));
        if (1ULL << (row * 8 + i) & block) break;
    }
    for (int i = col - 1; i >= 0; i--) {
        SET_BIT(attacks, (row * 8 + i));
        if (1ULL << (row * 8 + i) & block) break;
    }   

    return attacks;
}


// Magic numbers
uint64_t set_occupancy(int index, int bits_in_mask, uint64_t attack_mask) {
    
    // Initialize occupancy mask
    uint64_t occupancy = EMPTY_BOARD;

    for (int i = 0; i < bits_in_mask; i++) {
        
        int square = get_ls1b_index(attack_mask);
        POP_BIT(attack_mask, square);

        if (index & (1 << i)) {
            occupancy |= (1ULL << square);
        }
    }

    return occupancy;
}


uint64_t find_magic_number(Square square, int relevant_bits, Piece piece) {

    assert(piece == ROOK || piece == BISHOP);

    // Init possible occupancies array (4096 in the case of rooks)
    uint64_t occupancies[4096];

    // Initialize attack tables;
    uint64_t attacks[4096];

    // Initialize used attacks
    uint64_t used_attacks[4096];

    // Initialize attack mask for current piece
    uint64_t attack_mask = (piece == ROOK) ? get_rook_attacks(square) : get_bishop_attacks(square);

    // Initialize occupancy indices
    int occupancy_indexes = 1 << relevant_bits;
    for (int i = 0; i < occupancy_indexes; i++) {
        occupancies[i] = set_occupancy(i, relevant_bits, attack_mask);
        attacks[i] = (piece == ROOK) ? get_bishop_attacks_blocked(square, occupancies[i]) : get_rook_attacks_blocked(square, occupancies[i]);
    }

    // Check magic numbers
    int MAX_NUMBERS_GENERATED = 1000000000;
    for (int i = 0; i < MAX_NUMBERS_GENERATED; i++) {

        // Generate magic number candidate
        uint64_t magic_number = get_random_magic_number();

        // Discard invalid magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;
    
        // Initialize used attacks to 0
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        bool fail = false;
        for (int j = 0; j < occupancy_indexes && !fail; j++) {

            int magic_index = (occupancies[j] * magic_number) >> (64 - relevant_bits);

            if (used_attacks[magic_index] == 0ULL) {
                used_attacks[magic_index] = attacks[j];
            } else if (used_attacks[magic_index] != attacks[j]) {
                // Magic number doesn't work, try other one
                fail = true;
            }
        }

        if (!fail) {
            return magic_number;
        }
    }

    // If no suittable magic number was found
    printf("Magic number not found!");
    return 0ULL;
}


void init_magic_numbers() {

    for (int square = 0; square < 64; square++) {

        uint64_t magic = find_magic_number(square, bishop_relevant_bits[square], BISHOP);
        printf("%lu\n", magic);
    }
}


// Generate attack tables
void init_piece_attacks() {
    
    for (size_t square = 0; square < 64; square++) {
        
        // Initialize pawn attack table
        pawn_attacks[WHITE][square] = get_pawn_attacks(WHITE, square);
        pawn_attacks[BLACK][square] = get_pawn_attacks(BLACK, square);
    
        // Initialize knight attack table
        knight_attacks[square] = get_knight_attacks(square);
    
        // Initialize king attack table
        king_attacks[square] = get_king_attacks(square);
   }
}

