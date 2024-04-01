#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "chess/bitboard.h"
#include "chess/attack.h"
#include "chess/random.h"
#include "chess/movegen.h"
#include "chess/move.h"


int main() {

    // init_piece_attacks();

    // Position pos;
    // parse_fen(&pos, "r3k2r/pR1pqRP1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBRPrP/R3K2R w KQkq - 0 1 ");
    // print_position(pos);

    // generate_moves(&pos);

    int move1 = encode_move(d2, d4, Q, P, CAPTURE);
    int move2 = encode_move(h6, g1, r, K, CAPTURE | ENPASSANT);
    int move3 = encode_move(e4, d5, p, N, CASTLING | DOUBLE);

    MoveList list = {};
    push_move(&list, move1);
    push_move(&list, move2);
    push_move(&list, move3);

    // printf("src square: %s\n", square_to_coordinates[GET_MOVE_SRC(move)]);
    // printf("dest square: %s\n", square_to_coordinates[GET_MOVE_DEST(move)]);
    // printf("piece: %s\n", UNICODE_PIECES[GET_MOVE_PIECE(move)]);
    // printf("promotion: %s\n", UNICODE_PIECES[GET_MOVE_PROMOTION(move)]);
    // printf("capture flag: %s\n", IS_MOVE_CAPTURE(move) ? "true" : "false");
    // printf("double push flag: %s\n", IS_MOVE_DOUBLE(move) ? "true" : "false");
    // printf("enpassant flag: %s\n", IS_MOVE_ENPASSANT(move) ? "true" : "false");
    // printf("castling flag: %s\n", IS_MOVE_CASTLE(move) ? "true" : "false");

    // print_move(move1);
    print_move_list(&list);

    return 0;
}
