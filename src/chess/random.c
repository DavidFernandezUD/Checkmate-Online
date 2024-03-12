#include <stdlib.h>
#include <stdint.h>


// Generates 64 bit bitboard
uint64_t get_random_bitboard() {

    uint64_t num1, num2, num3, num4;

    num1 = (uint64_t) (rand() & 0xFFFF);
    num2 = (uint64_t) (rand() & 0xFFFF);
    num3 = (uint64_t) (rand() & 0xFFFF);
    num4 = (uint64_t) (rand() & 0xFFFF);

    // Composing bitboard from 4 random 32 bit numbers
    return num1 | (num2 << 16) | (num3 << 32) | (num4 << 48);
}


// Generate a random magic number candidate
uint64_t get_random_magic_number() {
    
    // This lowers the density of the random bitboards
    return get_random_bitboard() & get_random_bitboard() & get_random_bitboard();
}

