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

static int
chessboard_cmp_internal(chessboard_t *a, chessboard_t *b)
{
	ereport(NOTICE,(errcode(ERRCODE_CHARACTER_NOT_IN_REPERTOIRE),errmsg("Board A: %s \n Board B: %s\n \n", board(a), board(b))));
  if (strcmp(board(a), board(b)) < 0)
  {
    return -1;
  }
  if (strcmp(board(a), board(b)) > 0)
  {
    return 1;
  }
  return 0;
}

static bool
_chessboard_contains_internal(ArrayType *a, ArrayType *b)
{
	chessboard_t **_chessboard_1 = ARRPTR(a);
	chessboard_t **_chessboard_2 = ARRPTR(b);
	for (int i = 0; i < ARRNELEMS(b); ++i) {
		bool found = false;
		for (int j = 0; j < ARRNELEMS(a); ++j) {
			if (chessboard_cmp_internal(_chessboard_1[j], _chessboard_2[i]) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}
	return true;
}

static bool
_chessboard_overlap_internal(ArrayType *a, ArrayType *b)
{
	chessboard_t **_chessboard_1 = ARRPTR(a);
	chessboard_t **_chessboard_2 = ARRPTR(b);	
	for (int i = 0; i < ARRNELEMS(a); ++i) {
		for (int j = 0; j < ARRNELEMS(b); ++j) {
			if (chessboard_cmp_internal(_chessboard_1[i], _chessboard_2[j]) == 0) {
				return true;
			}
		}
	}
	return false;
}

static bool
_chessboard_eq_internal(ArrayType *a, ArrayType *b)
{
	chessboard_t **_chessboard_1 = ARRPTR(a);
	chessboard_t **_chessboard_2 = ARRPTR(b);
	if (ARRNELEMS(a) != ARRNELEMS(b)) {
		return false;
	}
	for (int i = 0; i < ARRNELEMS(a); ++i) {
		if (chessboard_cmp_internal(_chessboard_1[i], _chessboard_2[i]) != 0) {
			return false;
		}
	}
	return true;
}

PG_FUNCTION_INFO_V1(_chessboard_overlap);
Datum
_chessboard_overlap(PG_FUNCTION_ARGS)
{
	ArrayType *a = PG_GETARG_ARRAYTYPE_P(0);
	ArrayType *b = PG_GETARG_ARRAYTYPE_P(1);
	PG_RETURN_BOOL(_chessboard_overlap_internal(a, b));
}

PG_FUNCTION_INFO_V1(_chessboard_contains);
Datum
_chessboard_contains(PG_FUNCTION_ARGS)
{
	ArrayType *a = PG_GETARG_ARRAYTYPE_P(0);
	ArrayType *b = PG_GETARG_ARRAYTYPE_P(1);
	PG_RETURN_BOOL(_chessboard_contains_internal(a, b));
}

PG_FUNCTION_INFO_V1(_chessboard_contained);
Datum
_chessboard_contained(PG_FUNCTION_ARGS)
{
	ArrayType *a = PG_GETARG_ARRAYTYPE_P(0);
	ArrayType *b = PG_GETARG_ARRAYTYPE_P(1);
	PG_RETURN_BOOL(_chessboard_contains_internal(b, a)); 
}

PG_FUNCTION_INFO_V1(_chessboard_eq);
Datum
_chessboard_eq(PG_FUNCTION_ARGS)
{
	ArrayType *a = PG_GETARG_ARRAYTYPE_P(0);
	ArrayType *b = PG_GETARG_ARRAYTYPE_P(1);
	PG_RETURN_BOOL(_chessboard_eq_internal(a, b));	
}

PG_FUNCTION_INFO_V1(chessboard_cmp);
Datum
chessboard_cmp(PG_FUNCTION_ARGS)
{
	chessboard_t *a = PG_GETARG_CHESSBOARD_P(0);
	chessboard_t *b = PG_GETARG_CHESSBOARD_P(1);
	PG_RETURN_INT32(chessboard_cmp_internal(a, b));
}

/*****************************************************************************/