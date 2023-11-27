/*
 * chess_btree.c
 *
 * Btree functions for chess.c
 *
 */

#include <postgres.h>
#include <float.h>
#include <math.h>

#include "fmgr.h"
#include "utils/builtins.h"
#include "libpq/pqformat.h"

#include "chess.h"

#define opening(chessgame) ("null") // TODO
#define board(chessboard) (chessboard->piece_placement_data)

/* Index to support the hasOpening predicate */

static int
chess_opening_cmp_internal(chessgame_t *a, chessgame_t *b)
{
    return strcmp(opening(a), opening(b));
}

PG_FUNCTION_INFO_V1(chess_opening_eq);
Datum
chess_opening_eq(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chess_opening_cmp_internal(c, d) == 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_opening_ne);
Datum
chess_opening_ne(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chess_opening_cmp_internal(c, d) != 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_opening_cmp);
Datum
chess_opening_cmp(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  int result = chess_opening_cmp_internal(c, d);
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_INT32(result);
}


/*****************************************************************************/

/* Index to support the hasBoard predicate */

static int
chess_board_cmp_internal(chessboard_t *a, chessboard_t *b)
{
    return strcmp(board(a), board(b));
}

PG_FUNCTION_INFO_V1(chess_board_eq);
Datum
chess_board_eq(PG_FUNCTION_ARGS)
{
  chessboard_t *c = PG_GETARG_CHESSBOARD_P(0);
  chessboard_t *d = PG_GETARG_CHESSBOARD_P(1);
  bool result = chess_board_cmp_internal(c, d) == 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_board_ne);
Datum
chess_board_ne(PG_FUNCTION_ARGS)
{
  chessboard_t *c = PG_GETARG_CHESSBOARD_P(0);
  chessboard_t *d = PG_GETARG_CHESSBOARD_P(1);
  bool result = chess_board_cmp_internal(c, d) != 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_board_cmp);
Datum
chess_board_cmp(PG_FUNCTION_ARGS)
{
  chessboard_t *c = PG_GETARG_CHESSBOARD_P(0);
  chessboard_t *d = PG_GETARG_CHESSBOARD_P(1);
  int result = chess_board_cmp_internal(c, d);
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_INT32(result);
}

/*****************************************************************************/