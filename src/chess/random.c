#include <stdlib.h>
#include <stdint.h>


uint64_t get_random_bitboard() {

    uint64_t num1, num2, num3, num4;

    num1 = (uint64_t) (rand() & 0xFFFF);
    num2 = (uint64_t) (rand() & 0xFFFF);
    num3 = (uint64_t) (rand() & 0xFFFF);
    num4 = (uint64_t) (rand() & 0xFFFF);

    return num1 | (num2 << 16) | (num3 << 32) | (num4 << 48);
}

