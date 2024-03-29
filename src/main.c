#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/random.h"


int main() {

    // srand(time(NULL));
    init_magic_numbers();

    init_bishop_attacks();
    init_rook_attacks();

    uint64_t occupancy = 0ULL;

    SET_BIT(occupancy, f5);
    SET_BIT(occupancy, b5);
    SET_BIT(occupancy, e8);
    SET_BIT(occupancy, e4);

    print_bitboard(get_bishop_attacks(d5, occupancy));

    // print_bitboard(occupancy);

    // print_bitboard(get_bishop_attacks(d5, occupancy));

    // NOTES: mn = 64

    return 0;
}

