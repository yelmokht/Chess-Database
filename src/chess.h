#ifndef _CHESS_H
#define _CHESS_H

#include <postgres.h>

#define STARTING_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define STARTING_ACTIVE_COLOR 'w'
#define STARTING_CASTLING_AVAILABILITY "KQkq"
#define STARTING_EN_PASSANT_TARGET '-'
#define STARTING_HALFMOVE_CLOCK 0
#define STARTING_FULLMOVE_CLOCK 1

/**
 * @brief Structure containing all information representing a chess game state using SAN notation.
 */
typedef struct {
  const char* pgn;
} chessgame_t;

/* fmgr macros chessgame type */
#define DatumGetChessgameP(X)  ((chessgame_t *) DatumGetPointer(X))
#define ChessgamePGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_CHESSGAME_P(n) DatumGetChessgameP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSGAME_P(x) return ChessgamePGetDatum(x)

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

/* fmgr macros chessboard type */
#define DatumGetChessboardP(X)  ((chessboard_t *) DatumGetPointer(X))
#define ChessboardPGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_CHESSBOARD_P(n) DatumGetChessboardP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSBOARD_P(x) return ChessboardPGetDatum(x)

static chessgame_t *chessgame_make(char *san);
static chessboard_t *chessboard_make(char *piece_placement_data, char active_color, char *castling_availability, char *en_passant_target_square, int halfmove_clock, int fullmove_clock);

static chessgame_t *PGN_to_chessgame(char *pgn) 
static chessboard_t *FEN_to_chessboard(char* fen);

#endif