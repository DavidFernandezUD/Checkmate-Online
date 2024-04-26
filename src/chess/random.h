/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * 64 bit pseudorandom number generator and candidate magic number generator for sliding
 * piece attack table generation.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include <stdint.h>


#ifndef _RANDOM_H
#define _RANDOM_H


uint64_t get_random_magic_number();

uint64_t get_random_bitboard();


#endif // _RANDOM_H
