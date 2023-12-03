/*
 * chess_gin.c
 *
 * GIN functions for chess.c
 *
 */

#include <postgres.h>
#include <float.h>
#include <math.h>

#include "fmgr.h"
#include "utils/builtins.h"
#include "libpq/pqformat.h"

#include "chess.h"

#define board(chessboard) (chessboard->fen)

/******************************************************************************
 * GIN index functions to support the hasBoard predicate 
******************************************************************************/

static int
chess_board_cmp_internal(chessboard_t *a, chessboard_t *b)
{
  if (board(a) < board(b))
  {
    return -1;
  }
  if (board(a) > board(b))
  {
    return 1;
  }
  return 0;
}

PG_FUNCTION_INFO_V1(chess_board_intersection);
Datum
chess_board_intersection(PG_FUNCTION_ARGS)
{
}

PG_FUNCTION_INFO_V1(chess_board_contains);
Datum
chess_board_contains(PG_FUNCTION_ARGS)
{
}

PG_FUNCTION_INFO_V1(chess_board_contained_in);
Datum
chess_board_contained_in(PG_FUNCTION_ARGS)
{
}

PG_FUNCTION_INFO_V1(chess_board_eq);
Datum
chess_board_eq(PG_FUNCTION_ARGS)
{
}

PG_FUNCTION_INFO_V1(chess_board_cmp);
Datum
chess_board_cmp(PG_FUNCTION_ARGS)
{
  chessboard_t *c = PG_GETARG_CHESSGAME_P(0);
  chessboard_t *d = PG_GETARG_CHESSGAME_P(1);
  int result = chess_board_cmp_internal(c, d);
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_INT32(result);
}

/*****************************************************************************/