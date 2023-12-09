#ifndef _CHESS_H
#define _CHESS_H

#define MAX_PGN_LENGTH 4096

#include <postgres.h>
#include <utils/array.h>

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
#define DatumGetChessboardP(X) ((chessboard_t *) DatumGetPointer(X))
#define ChessboardPGetDatum(X) PointerGetDatum(X)
#define PG_GETARG_CHESSBOARD_P(n) DatumGetChessboardP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSBOARD_P(x) return ChessboardPGetDatum(x)

/* useful macros for accessing chessboard arrays */
#define ARRNELEMS(x) ArrayGetNItems(ARR_NDIM(x), ARR_DIMS(x))
#define ARRISEMPTY(x) (ARRNELEMS(x) == 0)

#endif