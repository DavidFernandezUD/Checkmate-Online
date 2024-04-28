/* 
 * +--------------------+
 * |  Checkmate-Online  |
 * +--------------------+
 * 
 * Position evaluation function for move searching.
 * 
 * Author: David Fernandez Fernandez <david.f@opendeusto.es>
 */

#include <stdint.h>
#include "bitboard.h"
#include "eval.h"


// Material scores for each piece type
const int material_score[] = {
    100,  // White pawn
    300,  // White knight
    350,  // White bishop
    500,  // white rook
   1000,  // White queen
  10000,  // White king
   -100,  // Black pawn
   -300,  // Black knight
   -350,  // Black bishop
   -500,  // Black rook
  -1000,  // Black queen
 -10000   // Black king
};


// Square based score multipliers for pawns
const int pawn_scores[] = {
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  20,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0  
};

// Square based score multipliers for knights
const int knight_scores[] = {
   -5,   0,   0,   0,   0,   0,   0,  -5,
   -5,   0,   0,  10,  10,   0,   0,  -5,
   -5,   5,  20,  20,  20,  20,   5,  -5,
   -5,  10,  20,  30,  30,  20,  10,  -5,
   -5,  10,  20,  30,  30,  20,  10,  -5,
   -5,   5,  20,  10,  10,  20,   5,  -5,
   -5,   0,   0,   0,   0,   0,   0,  -5,
   -5, -10,   0,   0,   0,   0, -10,  -5    
};

// Square based score multipliers for bishops
const int bishop_scores[] = {
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,  10,  10,   0,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,  10,   0,   0,   0,   0,  10,   0,
    0,  30,   0,   0,   0,   0,  30,   0,
    0,   0, -10,   0,   0, -10,   0,   0
};

// Square based score multipliers for rooks
const int rook_scores[] = {
   50,  50,  50,  50,  50,  50,  50,  50,
   50,  50,  50,  50,  50,  50,  50,  50,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,   0,  20,  20,   0,   0,   0
};

// Positional scores for queens
const int queen_scores[] = {
  -20, -10, -10,  -5,  -5, -10, -10, -20,
  -10,   0,   0,   0,   0,   0,   0, -10,
  -10,   0,   5,   5,   5,   5,   0, -10,
   -5,   0,   5,   5,   5,   5,   0,  -5,
    0,   0,   5,   5,   5,   5,   0,  -5,
  -10,   5,   5,   5,   5,   5,   0, -10,
  -10,   0,   5,   0,   0,   0,   0, -10,
  -20, -10, -10,  -5,  -5, -10, -10, -20,
};

// Positional scores for kings
const int king_scores[] = {
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   5,   5,   5,   5,   0,   0,
    0,   5,   5,  10,  10,   5,   5,   0,
    0,   5,  10,  20,  20,  10,   5,   0,
    0,   5,  10,  20,  20,  10,   5,   0,
    0,   0,   5,  10,  10,   5,   0,   0,
    0,   5,   5,  -5,  -5,   0,   5,   0,
    0,   0,   5,   0, -15,   0,  10,   0
};

// Utility to mirror squares vertically
const int mirror_squares[] = {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};