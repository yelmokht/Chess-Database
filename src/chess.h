#ifndef _CHESS_H
#define _CHESS_H

#define STARTING_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define STARTING_ACTIVE_COLOR 'w'
#define STARTING_CASTLING_AVAILABILITY "KQkq"
#define STARTING_EN_PASSANT_TARGET '-'
#define STARTING_HALFMOVE_CLOCK 0
#define STARTING_FULLMOVE_CLOCK 1

typedef struct {
  //TODO
} chessgame;

/**
 * @brief Structure containing all information representing a chess board state using FEN notation.
 */
typedef struct {
  char *piece_placement_data;
  char active_color;
  char *castling_availability;
  char *en_passant_target_square;
  int halfmove_clock;
  int fullmove_clock;
} chessboard_t;

static chessboard_t * chessboard_make(char *piece_placement_data, char active_color, char *castling_availability, char *en_passant_target_square, int halfmove_clock, int fullmove_clock);
chessboard_t * FEN_parsing(const char* fen);
void print_chessboard(chessboard_t *chessboard);


#endif
