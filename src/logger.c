/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Logging utilities.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

void log_msg(char* path, char* message) {

    FILE* file = fopen(path, "a");
    if (file == NULL) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed opening log file\n");
        exit(1);
    }

    fputs(message, file);
    fprintf(stderr, message);

    fclose(file);
}
