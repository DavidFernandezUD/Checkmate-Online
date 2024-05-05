/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Search function to find the best move from a tree of possible moves using
 * mini-max algorithms and alpha-beta pruning.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "makemove.h"
#include "eval.h"


#ifndef _SEARCH_H
#define _SEARCH_H


#define MAX_DEPTH 25


void search_position(Position* pos, int depth);


// TODO: Not sure if I should include half_move increments and decrements here as well
static inline int quiescence(Position* pos, int alpha, int beta, int* nodes) {

    (*nodes)++;

    int eval = evaluate(pos);

    if (eval >= beta) {
        return beta;
    }

    if (eval > alpha) {
        alpha = eval;
    }

    MoveList move_list;
    generate_moves(pos, &move_list);

    for (int i = 0; i < move_list.top; i++) {

        // Copy position before making move
        Position pos_cpy = *pos;

        // Only searching capture moves
        if (make_move(pos, move_list.moves[i], CAPTURE_MOVES) == 0) {
            *pos = pos_cpy;
            continue;
        }

        int score = -quiescence(pos, -beta, -alpha, nodes);

        *pos = pos_cpy;

        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}


static inline int negmax(Position* pos, int depth, int alpha, int beta, int* half_move, int* nodes, int* best_move) {

    // Base case
    if (depth == 0) {
        // Quiescence search
        return quiescence(pos, alpha, beta, nodes);

        // return evaluate(pos);
    }

    // Increment nodes counter
    (*nodes)++;

    // Check if king is checked
    int checked;
    if (pos->turn == WHITE) {
        checked = is_square_attacked(pos, get_ls1b_index(pos->bitboards[K]), BLACK);
    } else {
        checked = is_square_attacked(pos, get_ls1b_index(pos->bitboards[k]), WHITE);
    }

    int legal_moves = 0;

    // Current best move
    int current_best_move = 0;

    // Old value of alpha
    int old_alpha = alpha;

    // Generate possible moves in current position
    MoveList move_list;
    generate_moves(pos, &move_list);

    // Loop over possible moves
    for (int i = 0; i < move_list.top; i++) {

        // Copy position before making move
        Position pos_cpy = *pos;

        (*half_move)++;

        if (make_move(pos, move_list.moves[i], ALL_MOVES) == 0) {
            
            // Restore position
            (*half_move)--;

            // Restore position
            *pos = pos_cpy;

            // Skip to next move
            continue;
        }

        // Increment legal moves count
        legal_moves++;

        // get score  of current move
        int score = -negmax(pos, depth - 1, -beta, -alpha, half_move, nodes, best_move);

        (*half_move)--;

        // Restore position
        *pos = pos_cpy;

        // hard beta cutoff
        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {

            // Update new alpha
            alpha = score;

            // If root node, update current best move
            if (*half_move == 0) {
                current_best_move = move_list.moves[i];
            }
        }
    }

    // If no legal moves in current position
    if (legal_moves == 0) {
        
        // Checkmate
        if (checked) {
            // NOTE: We add half_move to prioritize checkmates in fewer moves
            return -98000 + (*half_move);
        } 
        
        // Stalemate
        else {
            return 0;
        }
    }

    if (old_alpha != alpha) {
        *best_move = current_best_move;
    }

    return alpha;
}


#endif // _SEARCH_H
