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


#ifndef _SEARCH_H
#define _SEARCH_H


#define MAX_DEPTH 25


void search_position(Position* pos, int depth);


#endif // _SEARCH_H
