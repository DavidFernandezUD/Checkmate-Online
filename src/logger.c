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
#include "logger.h"

void log(char* path, char* message) {

    FILE* file = fopen(path, "a");
    if (file == NULL) {
        fprintf(stderr, "\e[0;31m[ERROR]\e[0m Failed opening log file\n");
        exit(1);
    }

    fprintf(file, message);
    fprintf(stderr, message);

    fclose(file);
}
