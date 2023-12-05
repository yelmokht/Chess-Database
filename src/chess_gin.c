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
#include "utils/array.h"
#include "utils/builtins.h"
#include "libpq/pqformat.h"

#include "access/gin.h"
#include "access/stratnum.h"
#include "chess.h"

#define board(chessboard) (chessboard->fen)

/******************************************************************************
 * GIN index functions to support the hasBoard predicate 
******************************************************************************/

static bool
chess_board_contains_internal(ArrayType *a, ArrayType *b)
{
	int	i = 0;
	int j = 0;
	int n = 0;
	int na = ARRNELEMS(a);
	int nb = ARRNELEMS(b);
	int *da = ARRPTR(a);
	int *db = ARRPTR(b);

	while (i < na && j < nb)
	{
		if (da[i] < db[j])
    	{
      		i++;
    	}
		else if (da[i] == db[j])
		{
			n++;
			i++;
			j++;
		}
		else
    	{
      		break;
    	}
	}
	
	return (n == nb);
}

static bool
chess_board_overlap_internal(ArrayType *a, ArrayType *b)
{
	int na = ARRNELEMS(a);
	int nb = ARRNELEMS(b);
	int *da = ARRPTR(a);
	int *db = ARRPTR(b);
	int i = 0;
  int j = 0;
	while (i < na && j < nb)
	{
		if (da[i] < db[j])
    {
      i++;
    }
		else if (da[i] == db[j])
    {
      return true;
    }
		else
    {
      j++;
    }
	}

	return false;
}

static bool
chess_board_eq_internal(ArrayType *a, ArrayType *b)
{
	int na = ARRNELEMS(a);
	int nb = ARRNELEMS(b);
	int *da = ARRPTR(a);
	int *db = ARRPTR(b);
	bool result = false;
	if (na == nb)
	{
		result = true;
		for (int n = 0; n < na; n++)
		{
			if (da[n] != db[n])
			{
				result = false;
				break;
			}
		}
	}
}

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

/******************************************************************************/

PG_FUNCTION_INFO_V1(chess_board_overlap);
Datum
chess_board_overlap(PG_FUNCTION_ARGS)
{
  ArrayType *a = PG_GETARG_ARRAYTYPE_P_COPY(0);
	ArrayType *b = PG_GETARG_ARRAYTYPE_P_COPY(1);
	CHECKARRVALID(a);
	CHECKARRVALID(b);
	if (ARRISEMPTY(a) || ARRISEMPTY(b))
  {
    return false;
  }
	bool result = chess_board_overlap_internal(a, b);
	pfree(a);
	pfree(b);
	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_board_contains);
Datum
chess_board_contains(PG_FUNCTION_ARGS)
{
  	ArrayType *a = PG_GETARG_ARRAYTYPE_P_COPY(0);
	ArrayType *b = PG_GETARG_ARRAYTYPE_P_COPY(1);
	CHECKARRVALID(a);
	CHECKARRVALID(b);
	bool result = chess_board_contains_internal(a, b);
	pfree(a);
	pfree(b);
	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_board_contained);
Datum
chess_board_contained(PG_FUNCTION_ARGS)
{
  ArrayType *a = PG_GETARG_ARRAYTYPE_P_COPY(0);
	ArrayType *b = PG_GETARG_ARRAYTYPE_P_COPY(1);
	CHECKARRVALID(a);
	CHECKARRVALID(b);
	bool result = chess_board_contains_internal(b, a);
	pfree(a);
	pfree(b);
	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chess_board_eq);
Datum
chess_board_eq(PG_FUNCTION_ARGS)
{
	ArrayType *a = PG_GETARG_ARRAYTYPE_P_COPY(0);
	ArrayType *b = PG_GETARG_ARRAYTYPE_P_COPY(1);
	CHECKARRVALID(a);
	CHECKARRVALID(b);
  bool result = chess_board_eq_internal(a, b);
	pfree(a);
	pfree(b);
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