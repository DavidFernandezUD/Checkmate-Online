/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Universal Chess Interface (UCI) protocol implementation to allow the chess engine
 * to comunicate with general purpose chess user interfaces.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include <stdio.h>
#include <string.h>
#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "makemove.h"
#include "search.h"
#include "uci.h"


int parse_move(Position* pos, const char* move_string) {

    MoveList move_list;

    generate_moves(pos, &move_list);

    int src_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
    int dst_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;

    for (int i = 0; i < move_list.top; i++) {
        
        int move = move_list.moves[i];

        if (src_square == GET_MOVE_SRC(move) && dst_square == GET_MOVE_DST(move)) {

            int promoted_piece = GET_MOVE_PROMOTION(move);
            if (promoted_piece) {
                if ((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q') {
                    return move;
                } else if ((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r') {
                    return move;
                } else if ((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b') {
                    return move;
                } else if ((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n') {
                    return move;
                }

                // If the promoted piece doesn't match with this move's one, skip to next move in move_list
                continue;
            }

            return move;
        }
    }

    return 0;
}


void parse_position(Position* pos, const char* uci_command) {

    // Skip the "position " command keyword
    if (strlen(uci_command) > 9) {
        uci_command += 9;
    } else {
        // TODO: Handle errors better
        fprintf(stderr, "\e[0;33m[WARNING]\e[0m Invalid position command passed to parse_position\n");
        return;
    }

    const char* current = uci_command;

    // Parse "startpos"
    if (strncmp(current, "startpos", 8) == 0) {
        parse_fen(pos, START_POSITION);
    } else {

        // Parse FEN string
        current = strstr(uci_command, "fen");
        if (current != NULL) {

            // Skip "fen " command keyword
            current += 4;

            parse_fen(pos, current);
        } else {

            // Default to starting position
            parse_fen(pos, START_POSITION);
            
            // TODO: Handle errors better
            fprintf(stderr, "\e[0;33m[WARNING]\e[0m Invalid position passed to parse_position\n");
            return;
        }
    }

    // Parse moves after position
    current = strstr(uci_command, "moves");
    if (current != NULL) {

        // Skip "moves " command keyword
        current += 6;

        // Loop until no new move string is found
        while (*current >= 'a' && *current <= 'h') {

            int move = parse_move(pos, current);

            if (move) {
                make_move(pos, move, ALL_MOVES);
            } else {
                
                // TODO: Handle errors better
                fprintf(stderr, "\e[0;33m[WARNING]\e[0m Ilegal move in parse_position\n");
                return;
            }

            // Move pointer to next move
            if (GET_MOVE_PROMOTION(move)) {
                // Also skip promotion piece character if previous move was a promotion
                current += 6;
            } else {
                current += 5;
            }
        }
    }
}


void parse_go(Position* pos, const char* uci_command) {

    const char* current = strstr(uci_command, "depth");

    int depth = -1;

    if (current != NULL) {

        // Skip "depth " cammand keyword
        current += 6;

        depth = atoi(current);
        if (depth > 0 && depth <= MAX_DEPTH) {
            search_position(pos, depth);
        } else {
            fprintf(stderr, "\e[0;33m[WARNING]\e[0m Invalid go depth\n");
            return;
        }
    } else {
        fprintf(stderr, "\e[0;33m[WARNING]\e[0m Invalid go command\n");
        return;
    }
}


void uci_loop(Position* pos) {

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    const int BUFFER_LEN = 2048;
    char input_buffer[BUFFER_LEN];

    printf("uciok\n");

    while (1) {
        print_position(*pos);

        // Reset user input
        memset(input_buffer, 0 , sizeof(input_buffer));

        fflush(stdout);

        // Get user input
        if (!fgets(input_buffer, BUFFER_LEN, stdin)) {
            continue;
        } 
        
        else if (*input_buffer == '\n') {
            continue;
        }

        // UCI handshake
        else if (strncmp(input_buffer, "isready", 7) == 0) {
            printf("readyok\n");
            continue;
        }

        // Parse UCI position
        else if (strncmp(input_buffer, "position", 8) == 0) {
            parse_position(pos, input_buffer);
        }

        // Parse UCI ucinewgame command
        else if (strncmp(input_buffer, "ucinewgame", 10) == 0) {
            parse_position(pos, "position startpos");
        }

        // Parse UCI go command
        else if (strncmp(input_buffer, "go", 2) == 0) {
            parse_go(pos, input_buffer);
        }

        // Parse UCI quit command
        else if (strncmp(input_buffer, "quit", 4) == 0) {
            break;
        }

        // Parse UCI uci command
        else if (strncmp(input_buffer, "uci", 3) == 0) {
            printf("id CHESS-E\n");
            printf("uciok\n");
        }
    }
}
