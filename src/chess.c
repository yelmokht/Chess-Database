/*
 * chess.C 
 *
 * PostgreSQL Chess
 *
 */

#include <stdio.h>
#include <postgres.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "utils/builtins.h"
#include "libpq/pqformat.h"
#include "chess.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(getBoard);

Datum getBoard(PG_FUNCTION_ARGS) {
  //TODO
}

PG_FUNCTION_INFO_V1(getFirstMoves);

Datum getFirstMoves(PG_FUNCTION_ARGS) {
  //TODO
}

PG_FUNCTION_INFO_V1(hasOpening);

Datum hasOpening(PG_FUNCTION_ARGS) {
  //TODO
  PG_RETURN_BOOL(false);
}

PG_FUNCTION_INFO_V1(hasBoard);

Datum hasBoard(PG_FUNCTION_ARGS) {
  //TODO
  PG_RETURN_BOOL(false);
}
