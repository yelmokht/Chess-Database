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
chessgame_cmp_internal(chessgame_t *a, chessgame_t *b)
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

PG_FUNCTION_INFO_V1(chessgame_lt);
Datum
chessgame_lt(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chessgame_cmp_internal(c, d) < 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessgame_le);
Datum
chessgame_le(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chessgame_cmp_internal(c, d) <= 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessgame_eq);
Datum
chessgame_eq(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chessgame_cmp_internal(c, d) == 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessgame_ne);
Datum
chessgame_ne(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chessgame_cmp_internal(c, d) != 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessgame_ge);
Datum
chessgame_ge(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chessgame_cmp_internal(c, d) >= 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessgame_gt);
Datum
chessgame_gt(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  bool result = chessgame_cmp_internal(c, d) > 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessgame_like);
Datum
chessgame_like(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  int number_half_moves = chessgame_to_number(d);
  chessgame_t *e = truncate_chessgame(c, number_half_moves);
  bool result = chessgame_cmp_internal(e, d) == 0;
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessgame_cmp);
Datum
chessgame_cmp(PG_FUNCTION_ARGS)
{
  chessgame_t *c = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *d = PG_GETARG_CHESSGAME_P(1);
  int result = chessgame_cmp_internal(c, d);
  PG_FREE_IF_COPY(c, 0);
  PG_FREE_IF_COPY(d, 1);
  PG_RETURN_INT32(result);
}

/*****************************************************************************/