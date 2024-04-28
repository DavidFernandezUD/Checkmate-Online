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


int main() {

    init_piece_attacks();

    Position pos;
    parse_fen(&pos, "rnbqkbnr/pp1p1ppp/8/8/8/8/PPP2PPP/RNBQKBNR w KQkq - 0 1 ");

    print_position(pos);

    printf("Eval : %d\n", evaluate(&pos));

    // uci_loop(&pos);

    return 0;
}
