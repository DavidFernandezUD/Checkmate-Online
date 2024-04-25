#include <stdio.h>
#include <string.h>
#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "makemove.h"
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


void parse_position(Position* pos, char* uci_command) {

    // Skip the "position " command keyword
    uci_command += 9;

    char* current = uci_command;

    // Parse "startpos"
    if (strncmp(current, "startpos", 8) == 0) {
        parse_fen(pos, START_POSITION);
    }

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
        fprintf(stderr, "\e[0;33m[WARNING]\e[0m No position passed to parse_position\n");
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

                break;
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
