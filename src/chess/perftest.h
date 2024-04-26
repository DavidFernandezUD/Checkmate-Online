/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Performance and correctness testing utilities for the move generator. The perf_test
 * function is used to generate all posible positions from a given position with an specified
 * depth. The elapsed time and number of leaf nodes found, can be used for testing purposes.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include "bitboard.h"


#ifndef _PERF_H
#define _PERF_H


int get_time_ms();


void perf_test(Position* pos, int depth, long* nodes);


#endif // _PERF_H
