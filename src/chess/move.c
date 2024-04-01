#include <stdio.h>
#include <ctype.h>
#include "bitboard.h"
#include "move.h"


void print_move(int move) {
    #ifdef WIN64
        printf(
            "%s%s%c     %s      %s      %s      %s      %s\n",
            square_to_coordinates[GET_MOVE_SRC(move)], 
            square_to_coordinates[GET_MOVE_DEST(move)],
            GET_MOVE_PROMOTION(move) ? tolower(ASCII_PIECES[GET_MOVE_PROMOTION(move)]) : ' ',
            ASCII_PIECES[GET_MOVE_PIECE(move)],
            IS_MOVE_CAPTURE(move) ? "true " : "false",
            IS_MOVE_DOUBLE(move) ? "true " : "false",
            IS_MOVE_ENPASSANT(move) ? "true " : "false",
            IS_MOVE_CASTLE(move) ? "true " : "false"
        );
    #else
        printf(
            "%s%s%c     %s      %s      %s      %s      %s\n",
            square_to_coordinates[GET_MOVE_SRC(move)], 
            square_to_coordinates[GET_MOVE_DEST(move)],
            GET_MOVE_PROMOTION(move) ? tolower(ASCII_PIECES[GET_MOVE_PROMOTION(move)]) : ' ',
            UNICODE_PIECES[GET_MOVE_PIECE(move)],
            IS_MOVE_CAPTURE(move) ? "true " : "false",
            IS_MOVE_DOUBLE(move) ? "true " : "false",
            IS_MOVE_ENPASSANT(move) ? "true " : "false",
            IS_MOVE_CASTLE(move) ? "true " : "false"
        );
    #endif
}


void print_move_list(MoveList* list) {

    if (list->top > 0) {
        printf("Move      Piece  Capture    Double     Enpassant  Castling\n\n");
        
        for (int i = 0; i < list->top; i++) {
            int move = list->moves[i];
            print_move(move);
        }
    }
    printf("\nTotal moves: %d\n\n", list->top);
}
