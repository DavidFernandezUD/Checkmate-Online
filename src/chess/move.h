
#ifndef _MOVE_H
#define _MOVE_H


/* ====================== MOVE REPRESENTATION ======================

                BINARY                HEX             MOVE

    0000 0000 0000 0000 0011 1111   0x3f        source square
    0000 0000 0000 1111 1100 0000   0xfc0       destination square
    0000 0000 1111 0000 0000 0000   0xf000      piece
    0000 1111 0000 0000 0000 0000   0xf0000     promoted piece
    0001 0000 0000 0000 0000 0000   0x100000    capture flag
    0010 0000 0000 0000 0000 0000   0x200000    double push flag
    0100 0000 0000 0000 0000 0000   0x400000    enpassant flag
    1000 0000 0000 0000 0000 0000   0x800000    castling flag
*/


#define GET_MOVE_SRC(move) ((move) & 0x3f)
#define GET_MOVE_DEST(move) (((move) & 0xfc0) >> 6)
#define GET_MOVE_PIECE(move) (((move) & 0xf000) >> 12)
#define GET_MOVE_PROMOTION(move) (((move) & 0xf0000) >> 16)

#define IS_MOVE_CAPTURE(move) ((move) & 0x100000)
#define IS_MOVE_DOUBLE(move) ((move) & 0x200000)
#define IS_MOVE_ENPASSANT(move) ((move) & 0x400000)
#define IS_MOVE_CASTLE(move) ((move) & 0x800000)

typedef enum {CAPTURE = 1, DOUBLE = 2, ENPASSANT = 4, CASTLING = 8} MoveFlags;


// Move encoding
static inline int encode_move(
    int src_square,
    int dest_square,
    int piece,
    int promoted,
    MoveFlags flags) {
        
    return src_square | (dest_square << 6) | (piece << 12) | (promoted << 16) | (flags << 20);
}


// Move list
typedef struct {
    int moves[256];
    int top;
} MoveList;


static inline void push_move(MoveList* list, int move) {
    list->moves[list->top] = move;
    list->top++;
}


// Print utilities
void print_move(int move);

void print_move_list(MoveList* moves);


#endif // _MOVE_H
