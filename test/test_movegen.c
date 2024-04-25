#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../src/chess/bitboard.h"
#include "../src/chess/attack.h"
#include "../src/chess/random.h"
#include "../src/chess/movegen.h"
#include "../src/chess/move.h"
#include "../src/chess/makemove.h"
#include "../src/chess/perftest.h"


int main() {

    int n_tests = 7;

    // Positions to test movegen on
    char* positions[] = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 ",
        "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ",
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 ",
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ",
    };

    int depths[] = {
        6,
        5,
        7,
        5,
        5,
        5,
        5
    };

    // Expected nodes generated for each position at specified depth
    int expected_nodes[] = {
        119060324,
        193690690,
        178633661,
        15833292,
        15833292,
        89941194,
        164075551
    };

    // Initialize piece attack tables
    init_piece_attacks();

    Position pos;
    long nodes;

    // Timers
    int start, stop;
    float total_time_sec = 0;

    printf("\n========== TEST START ==========\n\n");

    for (int i = 0; i < n_tests; i++) {
        
        nodes = 0;

        parse_fen(&pos, positions[i]);

        start = get_time_ms();
        perf_test(&pos, depths[i], &nodes);
        stop = get_time_ms();

        total_time_sec += (stop - start) / 1000.f;

        // Assert results
        if (nodes == expected_nodes[i]) {
            printf("Test %d: \033[0;32m[PASSED]\033[0m in %4d ms\n", i + 1, stop - start);
        } else {
            printf("Test %d: \033[0;31m[FAILED]\033[0m in %4d ms\n\n", i + 1, stop - start);
            printf("    \033[0;31mposition: %s\n", positions[i]);
            printf("    expected nodes: %d\n", expected_nodes[i]);
            printf("    generated nodes: %ld\n\033[0m", nodes);

            printf("\n");
            printf(
                "+------------------------------+\n"
                "|          TEST FAILED         |\n"
                "+------------------------------+\n\n"
            );
            return 1;
        }
    }

    printf("\n\e[1;97mTime: %.2f s\e[0m\n\n", total_time_sec);
    
    printf(
        "+------------------------------+\n"
        "|          TEST PASSED         |\n"
        "+------------------------------+\n\n"
    );

    return 0;
}
