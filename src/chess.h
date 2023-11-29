#ifndef _CHESS_H
#define _CHESS_H

#define MAX_PGN_LENGTH 4096

#include <stdint.h>

/**
 * @brief Structure containing all information representing a chess game state using SAN notation.
 */
typedef struct {
  int32 length;
  char pgn[FLEXIBLE_ARRAY_MEMBER];
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
  int32 length;
  char fen[FLEXIBLE_ARRAY_MEMBER];
} chessboard_t;

/* fmgr macros chessboard type */
#define DatumGetChessboardP(X)  ((chessboard_t *) DatumGetPointer(X))
#define ChessboardPGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_CHESSBOARD_P(n) DatumGetChessboardP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSBOARD_P(x) return ChessboardPGetDatum(x)

static chessgame_t *chessgame_make(const char *pgn);
static chessboard_t *chessboard_make(const char *fen);
static chessgame_t *PGN_to_chessgame(char *pgn);
static char *chessgame_to_PGN(chessgame_t *chessgame);
static chessboard_t *FEN_to_chessboard(char *fen);
static char *chessboard_to_FEN(chessboard_t *chessboard);
static chessboard_t *chessgame_to_chessboard(chessgame_t *chessgame, uint16_t number_half_moves);
static chessgame_t  *truncate_chessgame(chessgame_t *chessgame, uint16_t number_half_moves);

#endif