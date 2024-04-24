#include "bitboard.h"


#ifndef _PERF_H
#define _PERF_H


int get_time_ms();


void perf_test(Position* pos, int depth, long* nodes);


#endif // _PERF_H
