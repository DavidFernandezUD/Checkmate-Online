#include <stdlib.h>
#include <sys/time.h>
#include "movegen.h"
#include "makemove.h"
#include "perf.h"

#ifdef WIN64
    #include <windows.h>
#else
    #include <sys/time.h>
#endif


int get_time_ms() {
    #ifdef WIN64
        return GetTickCount();
    #else
        struct timeval time_value;
        gettimeofday(&time_value, NULL);
        return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
    #endif
}


void perft_driver(Position* pos, int depth, long* nodes) {
    
    if (depth == 0) {
        (*nodes)++;
        return;
    }
    
    MoveList list;
    
    generate_moves(pos, &list);
    
    for (int i = 0; i < list.top; i++) {   
        Position pos_copy = *pos;

        if (make_move(pos, list.moves[i], ALL_MOVES)) {
            perft_driver(pos, depth - 1, nodes);
        }

        *pos = pos_copy; 
    }
}
