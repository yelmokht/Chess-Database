/*
 * chess_btree.c
 *
 * Btree functions for chess.c
 *
 */

#include <stdlib.h>
#include "postgres.h"
#include "chess.h"

#define opening(chessgame) (chessgame->pgn)

/******************************************************************************
 * Btree index functions to support the hasOpening predicate 
******************************************************************************/

static int
chess_opening_cmp_internal(chessgame_t *a, chessgame_t *b)
{
  int cmp_result = strcmp(opening(a), opening(b));
  if (cmp_result < 0)
  {
    return -1;
  }
  if (cmp_result > 0)
  {
    return 1;
  }
  return 0;
}

PG_FUNCTION_INFO_V1(chess_opening_lt);
Datum
chess_opening_lt(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chess_opening_cmp_internal(c, d) < 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_opening_le);
Datum
chess_opening_le(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chess_opening_cmp_internal(c, d) <= 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
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

PG_FUNCTION_INFO_V1(chess_opening_ge);
Datum
chess_opening_ge(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chess_opening_cmp_internal(c, d) >= 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_opening_gt);
Datum
chess_opening_gt(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chess_opening_cmp_internal(c, d) > 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_opening_like);
Datum
chess_opening_like(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);

  bool result = chess_opening_cmp_internal(chessgame_truncated_internal(c, chessgame_to_number_internal(d)), d) == 0;
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