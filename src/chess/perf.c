#include <stdlib.h>
#include <sys/time.h>
#include "perf.h"


int get_time_ms() {
    struct timeval time_value;
    gettimeofday(&time_value, NULL);
    return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
}
