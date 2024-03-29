#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/random.h"


int main() {

    init_piece_attacks();

    uint64_t occupancy = 0ULL;

    SET_BIT(occupancy, f7);
    SET_BIT(occupancy, b5);
    SET_BIT(occupancy, e8);
    SET_BIT(occupancy, e4);

    print_bitboard(get_bishop_attacks(d5, occupancy));

    return 0;
}

