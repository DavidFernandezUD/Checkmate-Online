#include <stdio.h>
#include <stdint.h>


#define EMPTY_BOARD 0ULL

#define GET_BIT(bitboard, index) ((bitboard) & (1ULL << (index)))
#define SET_BIT(bitboard, index) ((bitboard) |= (1ULL << (index)))
#define POP_BIT(bitboard, index) (GET_BIT((bitboard), (index)) ? bitboard ^= (1ULL << (index)) : 0)

// Usefull bitboard masks
const uint64_t NOT_A_COL = 18374403900871474942ULL;
const uint64_t NOT_AB_COL = 18229723555195321596ULL;
const uint64_t NOT_H_COL = 9187201950435737471ULL;
const uint64_t NOT_GH_COL = 4557430888798830399ULL;


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

typedef  enum {WHITE, BLACK} Color;


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


// <========= ATTACK TABLES =========>
uint64_t pawn_attacks[2][64];
uint64_t knight_attacks[64];
uint64_t king_attacks[64];


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

int main() {

    init_piece_attacks();

    uint64_t block = EMPTY_BOARD;
    SET_BIT(block, d2);
    SET_BIT(block, d6);
    SET_BIT(block, c4);
    SET_BIT(block, h4);

    uint64_t bitboard = EMPTY_BOARD;
    SET_BIT(bitboard, d4);

    printf("Rook:\n");
    print_bitboard(bitboard);

    printf("Blockers:\n");
    print_bitboard(block);

    printf("Blocked Attacks:\n");
    print_bitboard(get_rook_attacks_blocked(d4, block));

    return 0;
}

