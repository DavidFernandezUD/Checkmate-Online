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


// Magic numbers
uint64_t bishop_magic_numbers[64] = {
    2306441229572768042ULL,
    4755890013980525568ULL,
    4901086412658835524ULL,
    27097464877105152ULL,
    669910739179274752ULL,
    22676329960379392ULL,
    576610577944674320ULL,
    6791717955043842ULL,
    11606341423070777412ULL,
    4616024691381249ULL,
    1189020831450398724ULL,
    9511621105778690048ULL,
    9511613687295901718ULL,
    20547674746912804ULL,
    288265697968263184ULL,
    288240271791328353ULL,
    180144029286009348ULL,
    254734858285756576ULL,
    1302785987448832ULL,
    4611861940564721682ULL,
    22024730968204ULL,
    1276533268283392ULL,
    20875881826222080ULL,
    72605264528128ULL,
    579099599537438976ULL,
    3377699720693888ULL,
    4790026164502540ULL,
    144467238492184576ULL,
    576466250014655492ULL,
    777127356006656ULL,
    10415682240389122ULL,
    9223934986810040896ULL,
    10376296943079964672ULL,
    27339356626788450ULL,
    2315976142735278180ULL,
    16432509179026507264ULL,
    1171551771781234944ULL,
    18416819768608ULL,
    217298819728021632ULL,
    5910974736430530628ULL,
    9228040866088615936ULL,
    144118555363774724ULL,
    1170935972137816082ULL,
    2341871806534648064ULL,
    4505936358016404ULL,
    284842281402368ULL,
    9242022044353169540ULL,
    37720120782954504ULL,
    36169612891259424ULL,
    5771364056883857632ULL,
    4755801241401034240ULL,
    2667399921384161329ULL,
    4503599628567104ULL,
    14122745888ULL,
    2307114113409491080ULL,
    9295476085259141394ULL,
    325385081760268296ULL,
    90085186700853248ULL,
    13836183955189011220ULL,
    3494793322952721440ULL,
    40532396715622400ULL,
    2262050ULL,
    2306124624853076056ULL,
    155656761633996800ULL
};

uint64_t rook_magic_numbers[64] = {
    180146184118141000ULL,
    153122664439874560ULL,
    2595486429622697984ULL,
    566250770041024ULL,
    39418084997464064ULL,
    18577915406974992ULL,
    13845894841887916560ULL,
    4686603579648155648ULL,
    2895345393952ULL,
    137707962500ULL,
    41955388064817ULL,
    289358477229293572ULL,
    288511851136813061ULL,
    9649324850020992ULL,
    9223517172410876424ULL,
    137473819012ULL,
    4412011462656ULL,
    10403315968157156354ULL,
    2336864927744ULL,
    35460349952064ULL,
    9899916918784ULL,
    72375352965493032ULL,
    6052846695346077696ULL,
    2306415585262567424ULL,
    5764607548804104576ULL,
    38023240ULL,
    4503703243458560ULL,
    9223372054103851016ULL,
    4613410190134348292ULL,
    2306027744919027712ULL,
    2287155991019520ULL,
    1154049070961033472ULL,
    144116287855958528ULL,
    9585922424554127656ULL,
    10133167919087700ULL,
    72624976398680064ULL,
    274886950912ULL,
    54060856706072578ULL,
    1297037837293604868ULL,
    9224638674252070976ULL,
    146932137235321728ULL,
    14123308244201968816ULL,
    2269394214453760ULL,
    5206161169596846082ULL,
    293015451024818176ULL,
    9586052544356417584ULL,
    5188322692591919108ULL,
    9601887206155330578ULL,
    4611828956021137920ULL,
    288266660312252672ULL,
    5765733427236053312ULL,
    585503149352091920ULL,
    290553818301596161ULL,
    554051310113ULL,
    11530640013138462736ULL,
    151019537ULL,
    14987979834767442216ULL,
    13707207233241092ULL,
    15997913976428102196ULL,
    163258922476568880ULL,
    18015086778023952ULL,
    2306757813726089252ULL,
    72092800959152384ULL,
    1153273348327735681ULL 
};


// Sliding piece attack masks
uint64_t bishop_attack_masks[64];
uint64_t rook_attack_masks[64];


// Sliding piece attack tables [square][occupancy]
uint64_t bishop_attacks[64][512];
uint64_t rook_attacks[64][4096];


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
uint64_t get_bishop_attack_mask(Square square) {
    
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
uint64_t get_rook_attack_mask(Square square) {
    
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
