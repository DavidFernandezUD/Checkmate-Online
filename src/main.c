#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/random.h"


int main() {

    printf("Hello, World!\n");
    
    srand(time(NULL));

    init_piece_attacks();

    print_bitboard(get_random_bitboard());

    return 0;
}

