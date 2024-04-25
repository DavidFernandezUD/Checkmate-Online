#include <stdlib.h>
#include "movegen.h"
#include "makemove.h"
#include "perftest.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif


int get_time_ms() {
    #ifdef _WIN32
        return GetTickCount();
    #else
        struct timeval time_value;
        gettimeofday(&time_value, NULL);
        return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
    #endif
}


void perf_test(Position* pos, int depth, long* nodes) {
    
    if (depth == 0) {
        (*nodes)++;
        return;
    }
    
    MoveList list;
    
    generate_moves(pos, &list);
    
    for (int i = 0; i < list.top; i++) {   
        Position pos_copy = *pos;

        if (make_move(pos, list.moves[i], ALL_MOVES)) {
            perf_test(pos, depth - 1, nodes);
        }

        *pos = pos_copy;
    }
}