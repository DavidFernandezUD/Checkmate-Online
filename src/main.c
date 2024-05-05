/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Chess module testing placeholder.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/attack.h"
#include "chess/random.h"
#include "chess/movegen.h"
#include "chess/move.h"
#include "chess/makemove.h"
#include "chess/perftest.h"
#include "chess/uci.h"
#include "chess/eval.h"
#include "chess/search.h"


int main() {

    init_piece_attacks();

    Position pos;
    parse_fen(&pos, TRICKY_POSITION);

    uci_loop(&pos);

    return 0;
}
