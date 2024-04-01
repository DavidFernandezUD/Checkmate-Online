#include <stdio.h>
#include <ctype.h>
#include "bitboard.h"
#include "move.h"


void print_move(int move) {
    #ifdef WIN64
        printf(
            "%s%s%c\n",
            square_to_coordinates[GET_MOVE_SRC(move)], 
            square_to_coordinates[GET_MOVE_DEST(move)],
            tolower(ASCII_PIECES[GET_MOVE_PROMOTION(move)]) // NOTE: Promotion piece in lower case for UCI protocol
        );
    #else
        printf(
            "%s%s%s\n",
            square_to_coordinates[GET_MOVE_SRC(move)], 
            square_to_coordinates[GET_MOVE_DEST(move)],
            UNICODE_PIECES[GET_MOVE_PROMOTION(move)]
        );
    #endif
}


void print_move_list(MoveList* list) {

    for (int i = 0; i < list->top; i++) {
        int move = list->moves[i];
        print_move(move);
    }
}
