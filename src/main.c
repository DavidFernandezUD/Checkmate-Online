#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/random.h"


int main() {
    
    srand(time(NULL));

    init_piece_attacks();

    print_bitboard(get_random_magic_number());
    print_bitboard(get_random_magic_number());
    print_bitboard(get_random_magic_number());
    print_bitboard(get_random_magic_number());
    print_bitboard(get_random_magic_number());

    return 0;
}

