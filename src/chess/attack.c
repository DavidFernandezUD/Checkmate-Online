#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "attack.h"
#include "random.h"
#include "bitboard.h"


// Magic numbers
uint64_t bishop_magic_numbers[64] = {
    2324140016605872192ULL, 2863351642589184ULL, 505537856563847300ULL, 650779221421588480ULL,
    117676400466530304ULL, 4902326816661966945ULL, 739627864424736ULL, 354077439493136ULL,
    9228229687952081168ULL, 594483951217934608ULL, 108693364493787145ULL, 1441156295993524224ULL,
    9529617980820815872ULL, 2323859641173540868ULL, 9362376240464898ULL, 144135005625453664ULL,
    9529757566452761088ULL, 4756935971289732097ULL, 4900479486334009856ULL, 1171076692414955520ULL,
    9149087804836177ULL, 741405230394057810ULL, 72374255602369156ULL, 4611760785840408608ULL,
    9183671005414404ULL, 580973422968914944ULL, 1130300134917136ULL, 45044792383520776ULL,
    9009673256587264ULL, 146235189125121ULL, 2819173617045760ULL, 289361778216077568ULL,
    40542154946511872ULL, 2306973461855211524ULL, 9512165569118470280ULL, 9264049586239439136ULL,
    1126999721771072ULL, 633881338445889ULL, 4630443691095624832ULL, 72199432145338880ULL,
    290408575864960ULL, 5950566056329738ULL, 4648278870414339136ULL, 9516669246063519744ULL,
    432350000963388416ULL, 4616506311868153985ULL, 4901059903868665929ULL, 2447532244336674ULL,
    1189232335220899968ULL, 9800996106887503874ULL, 18049875006750724ULL, 941429352166597138ULL,
    344147378177ULL, 2310506046887624720ULL, 5068783098036288ULL, 4645445225808128ULL,
    90355864149573632ULL, 4629709492212207744ULL, 5502264673280ULL, 36029351074071040ULL,
    4648278315696784512ULL, 9665855126342992136ULL, 1298455268859446272ULL, 4557510076352000ULL
};

uint64_t rook_magic_numbers[64] = {
    180144535121444864ULL, 18031991232413704ULL, 5224185600801902593ULL, 9403520935400706048ULL,
    2341874555145946112ULL, 144124539562099200ULL, 36030996058996864ULL, 6953571158386295040ULL,
    4107423736162975744ULL, 281616714825856ULL, 891994274214715393ULL, 4909064881212231680ULL,
    2594636370215829520ULL, 288793360532111632ULL, 6936950809624355072ULL, 36591751544307812ULL,
    126101339360338048ULL, 2400418876268478528ULL, 4611827306208432128ULL, 288266660303866112ULL,
    324400460481955840ULL, 10376857041237967488ULL, 4755805604571251216ULL, 120946346198081ULL,
    11565244127629025291ULL, 12472860979240960ULL, 4620693357269614724ULL, 154547358695882784ULL,
    1172062354926733312ULL, 18581748657029248ULL, 577041311631213128ULL, 288231003216953644ULL,
    2954431726455095328ULL, 18015154430019586ULL, 63050463511062656ULL, 4504149517926656ULL,
    9270660935960233988ULL, 585470152737293312ULL, 2455869190406424064ULL, 585608864074830080ULL,
    35735201677312ULL, 9223447362008383520ULL, 4620693286535839808ULL, 10414855681992622090ULL,
    6919780965967725568ULL, 9295434028972802176ULL, 1441433364325269508ULL, 76561539477405713ULL,
    77516124848640ULL, 6138688042063921408ULL, 648553531803963648ULL, 180223160674681984ULL,
    2305983781062049920ULL, 2315131700625156352ULL, 17600913443840ULL, 4541260115805696ULL,
    3413163266572289ULL, 195062433741046289ULL, 4620974967845028249ULL, 153721689887539209ULL,
    2533343543691265ULL, 2306124501370406913ULL, 1196286523050426500ULL, 11530354147112272002ULL
};


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

const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};


// Precalculated attack tables
uint64_t pawn_attacks[2][64];
uint64_t knight_attacks[64];
uint64_t king_attacks[64];


// Sliding piece attack masks
uint64_t bishop_attack_masks[64];
uint64_t rook_attack_masks[64];


// Sliding piece attack tables [square][occupancy]
uint64_t bishop_attacks[64][512];
uint64_t rook_attacks[64][4096];


// Pawn attacks
uint64_t get_pawn_attacks(Color color, Square square) {

    uint64_t bitboard = EMPTY_BITBOARD;
    SET_BIT(bitboard, square);

    uint64_t attacks = EMPTY_BITBOARD;
    
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
    
    uint64_t bitboard = EMPTY_BITBOARD;
    SET_BIT(bitboard, square);

    uint64_t attacks = EMPTY_BITBOARD;
    
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
     
    uint64_t bitboard = EMPTY_BITBOARD;
    SET_BIT(bitboard, square);

    uint64_t attacks = EMPTY_BITBOARD;
    
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
uint64_t get_bishop_attack_mask(Square square) {
    
    uint64_t attacks = EMPTY_BITBOARD;
    
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
    
    uint64_t attacks = EMPTY_BITBOARD;
    
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
uint64_t get_rook_attack_mask(Square square) {
    
    uint64_t attacks = EMPTY_BITBOARD;
    
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
    
    uint64_t attacks = EMPTY_BITBOARD;
    
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
    uint64_t occupancy = EMPTY_BITBOARD;

    for (int i = 0; i < bits_in_mask; i++) {
        
        int square = get_ls1b_index(attack_mask);
        POP_BIT(attack_mask, square);

        if (index & (1 << i)) {
            occupancy |= (1ULL << square);
        }
    }

    return occupancy;
}


uint64_t find_magic_number(Square square, int relevant_bits, IsBishop piece) {

    assert(piece == ROOK || piece == BISHOP);

    // Init possible occupancies
    uint64_t occupancies[4096];

    // Initialize attack tables;
    uint64_t attacks[4096];

    // Initialize used attacks
    uint64_t used_attacks[4096];

    // Initialize attack mask for current piece
    uint64_t attack_mask = (piece == ROOK) ? get_rook_attack_mask(square) : get_bishop_attack_mask(square);

    // Initialize occupancy indices
    int occupancy_indexes = 1 << relevant_bits;
    for (int i = 0; i < occupancy_indexes; i++) {
        occupancies[i] = set_occupancy(i, relevant_bits, attack_mask);
        attacks[i] = (piece == ROOK) ? get_rook_attacks_blocked(square, occupancies[i]) : get_bishop_attacks_blocked(square, occupancies[i]);
    }

    // Check magic numbers
    int MAX_TRIES = 1000000000;
    for (int i = 0; i < MAX_TRIES; i++) {

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
        
        // Initialize rook magic numbers
        rook_magic_numbers[square] = find_magic_number(square, rook_relevant_bits[square], ROOK);

        // Initialize bishop magic numbers
        bishop_magic_numbers[square] = find_magic_number(square, bishop_relevant_bits[square], BISHOP);
    }
}


void init_bishop_attacks() {

    for (int square = 0; square < 64; square++) {
        
        bishop_attack_masks[square] = get_bishop_attack_mask(square);

        // init occupancy indicies
        int occupancy_indices = (1 << bishop_relevant_bits[square]);

        // loop over occupancy indicies
        for (int i = 0; i < occupancy_indices; i++) {

            uint64_t occupancy = set_occupancy(i, bishop_relevant_bits[square], bishop_attack_masks[square]);

            int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);

            bishop_attacks[square][magic_index] = get_bishop_attacks_blocked(square, occupancy);
        }
    }
}


void init_rook_attacks() {

    for (int square = 0; square < 64; square++) {

        rook_attack_masks[square] = get_rook_attack_mask(square);
        
        // init occupancy indicies
        int occupancy_indices = (1 << rook_relevant_bits[square]);

        // loop over occupancy indicies
        for (int i = 0; i < occupancy_indices; i++) {

            uint64_t occupancy = set_occupancy(i, rook_relevant_bits[square], rook_attack_masks[square]);
            int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);
            rook_attacks[square][magic_index] = get_rook_attacks_blocked(square, occupancy);
        }
    }
}


// TODO: Make static inline
uint64_t get_bishop_attacks(int square, uint64_t occupancy) {

    occupancy &= bishop_attack_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    return bishop_attacks[square][occupancy];
}


uint64_t get_rook_attacks(int square, uint64_t occupancy) {

    occupancy &= rook_attack_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];
    
    return rook_attacks[square][occupancy];
}


uint64_t get_queen_attacks(int square, uint64_t occupancy) {

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


// Generate attack tables
void init_piece_attacks() {
    
    for (int square = 0; square < 64; square++) {
        
        // Initialize pawn attack table
        pawn_attacks[WHITE][square] = get_pawn_attacks(WHITE, square);
        pawn_attacks[BLACK][square] = get_pawn_attacks(BLACK, square);
    
        // Initialize knight attack table
        knight_attacks[square] = get_knight_attacks(square);
    
        // Initialize king attack table
        king_attacks[square] = get_king_attacks(square);

        // Initialize bishop attack table
        init_bishop_attacks();

        // Initialize rook attack table
        init_rook_attacks();
   }
}
