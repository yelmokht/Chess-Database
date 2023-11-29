/*
 * PostgreSQL Chess
 *
 * chess.c
 *
 */

#include <stdio.h>
#include <postgres.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

#include "utils/builtins.h"
#include "libpq/pqformat.h"
#include "varatt.h"
#include "smallchesslib.h"
#include "chess.h"

PG_MODULE_MAGIC;

/******************************************************************************
 * Constructors
******************************************************************************/

/**
 * @brief Constructs a chessgame
 * @param pgn PGN notation of the chessgame
 * @return *chessgame_t Pointer to the chessgame
 */
static chessgame_t *
chessgame_make(const char *pgn)
{
  chessgame_t *chessgame = (chessgame_t *) palloc(VARHDRSZ + strlen(pgn) + 1);
  SET_VARSIZE(chessgame, VARHDRSZ + strlen(pgn) + 1);
  memcpy(chessgame->pgn, pgn, strlen(pgn) + 1);
  return chessgame;
}

/**
 * @brief Constructs a chessboard
 * @param fen FEN notation of the chessboard
 * @return *chessboard_t Pointer to the chessboard
 */
static chessboard_t *
chessboard_make(const char *fen)
{
  chessboard_t *chessboard = (chessboard_t *) palloc(VARHDRSZ + strlen(fen) + 1);
  SET_VARSIZE(chessboard, VARHDRSZ + strlen(fen) + 1);
  memcpy(chessboard->fen, fen, strlen(fen) + 1);
  return chessboard;
}

/******************************************************************************
 * Functions for data types
******************************************************************************/

/**
  * @brief Converts PGN to a chessgame
  * @param pgn PGN notation
  * @return *chessgame_t Pointer to the chessgame
  */
static chessgame_t *
PGN_to_chessgame(char *pgn) //this function should handle errors
{
  return chessgame_make(pgn);
}

/**
  * @brief Converts chessgame to PGN
  * @param chessgame Pointer to the chessgame
  * @return *char Pointer to the PGN notation
  */
static char *
chessgame_to_PGN(chessgame_t *chessgame)
{
  return psprintf("%s", chessgame->pgn);
}

/**
  * @brief Converts FEN to a chessboard
  * @param fen FEN notation
  * @return *chessboard_t Pointer to the chessboard
  */
static chessboard_t *
FEN_to_chessboard(char *fen)
{
  return chessboard_make(fen);
}

/**
  * @brief Converts chessboard to FEN
  * @param chessboard Pointer to the chessboard
  * @return *char Pointer to the FEN notation
  */
static char *
chessboard_to_FEN(chessboard_t *chessboard)
{
  return psprintf("%s", chessboard->fen);
}

static chessboard_t *
chessgame_to_chessboard(chessgame_t *chessgame, uint16_t number_half_moves)
{
  SCL_Record record;
  SCL_Board board;
  char *fen = (char *) malloc(sizeof(char) * SCL_FEN_MAX_LENGTH);
  SCL_recordFromPGN(record, chessgame->pgn);
  SCL_recordApply(record, board, number_half_moves);
  SCL_boardToFEN(board, fen);
  chessboard_t *chessboard = FEN_to_chessboard(fen);
  free(fen);
  return chessboard;
}

/**
  * @brief Truncates the chessgame to its first N half-moves.
  * @param chessgame Pointer to the chessgame
  * @param number_half_moves Number of half-moves
  * @return *chessgame_t Pointer to the new truncated chessgame
  */
static chessgame_t * 
truncate_chessgame(chessgame_t *chessgame, uint16_t number_half_moves)
{
  char delimeter = ' ';
  uint16_t i = 0;
  uint16_t counter = 0;
  char *truncated_pgn = (char *) malloc(sizeof(char) * MAX_PGN_LENGTH);
  while (chessgame->pgn[i] != '\0' && counter < number_half_moves) {
    if(chessgame->pgn[i] == delimeter) {
      counter += 1;
    }
    truncated_pgn[i] = chessgame->pgn[i];
    i += 1;
  }
  truncated_pgn[i] = '\0';
  chessgame_t *truncated_chessgame = PGN_to_chessgame(truncated_pgn);
  free(truncated_pgn);
  return truncated_chessgame;
}

/******************************************************************************
 * Input/output for chessgame
******************************************************************************/

PG_FUNCTION_INFO_V1(chessgame_in);
Datum
chessgame_in(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);
  PG_RETURN_CHESSGAME_P(PGN_to_chessgame(str)); 
}

PG_FUNCTION_INFO_V1(chessgame_out);
Datum
chessgame_out(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame = PG_GETARG_CHESSGAME_P(0);
  char *result = chessgame_to_PGN(chessgame); 
  PG_FREE_IF_COPY(chessgame, 0);
  PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(chessgame_recv);
Datum
chessgame_recv(PG_FUNCTION_ARGS)
{
  StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);
  PG_RETURN_CHESSGAME_P(PGN_to_chessgame((char*)pq_getmsgstring(buf)));
}

PG_FUNCTION_INFO_V1(chessgame_send);
Datum
chessgame_send(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame = PG_GETARG_CHESSGAME_P(0);
  StringInfoData buf;
  pq_begintypsend(&buf);
  pq_sendstring(&buf, chessgame->pgn);
  PG_FREE_IF_COPY(chessgame, 0);
  PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(chessgame_cast_from_text);
Datum
chessgame_cast_from_text(PG_FUNCTION_ARGS)
{
  text *txt = PG_GETARG_TEXT_P(0);
  char *str = DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(txt)));
  PG_RETURN_CHESSGAME_P(PGN_to_chessgame(str));
}

PG_FUNCTION_INFO_V1(chessgame_cast_to_text);
Datum
chessgame_cast_to_text(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame  = PG_GETARG_CHESSGAME_P(0);
  text *out = (text *)DirectFunctionCall1(textin, PointerGetDatum(chessgame_to_PGN(chessgame)));
  PG_FREE_IF_COPY(chessgame, 0);
  PG_RETURN_TEXT_P(out);
}

/******************************************************************************
 * Input/output for chessboard
******************************************************************************/

PG_FUNCTION_INFO_V1(chessboard_in);
Datum
chessboard_in(PG_FUNCTION_ARGS)
{
  char *str = PG_GETARG_CSTRING(0);
  PG_RETURN_CHESSBOARD_P(FEN_to_chessboard(str));
}

PG_FUNCTION_INFO_V1(chessboard_out);
Datum
chessboard_out(PG_FUNCTION_ARGS)
{
  chessboard_t *chessboard = PG_GETARG_CHESSBOARD_P(0);
  char *result = chessboard_to_FEN(chessboard);
  PG_FREE_IF_COPY(chessboard, 0);
  PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(chessboard_recv);
Datum
chessboard_recv(PG_FUNCTION_ARGS)
{
  StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);
  PG_RETURN_CHESSBOARD_P(FEN_to_chessboard((char*)pq_getmsgstring(buf)));
}

PG_FUNCTION_INFO_V1(chessboard_send);
Datum
chessboard_send(PG_FUNCTION_ARGS)
{
  chessboard_t *chessboard = PG_GETARG_CHESSBOARD_P(0);
  StringInfoData buf;
  pq_begintypsend(&buf);
  pq_sendstring(&buf, chessboard->fen);
  PG_FREE_IF_COPY(chessboard, 0);
  PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(chessboard_cast_from_text);
Datum
chessboard_cast_from_text(PG_FUNCTION_ARGS)
{
  text *txt = PG_GETARG_TEXT_P(0);
  char *str = DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(txt)));
  PG_RETURN_CHESSBOARD_P(FEN_to_chessboard(str));
}

PG_FUNCTION_INFO_V1(chessboard_cast_to_text);
Datum
chessboard_cast_to_text(PG_FUNCTION_ARGS)
{
  chessboard_t *chessboard  = PG_GETARG_CHESSBOARD_P(0);
  text *out = (text *)DirectFunctionCall1(textin, PointerGetDatum(chessboard_to_FEN(chessboard)));
  PG_FREE_IF_COPY(chessboard, 0);
  PG_RETURN_TEXT_P(out);
}

/******************************************************************************
 * Constructors
******************************************************************************/

PG_FUNCTION_INFO_V1(chessgame_constructor);
Datum
chessgame_constructor(PG_FUNCTION_ARGS)
{
  char *pgn = PG_GETARG_CSTRING(0);
  PG_RETURN_CHESSGAME_P(PGN_to_chessgame(pgn));
}

PG_FUNCTION_INFO_V1(chessboard_constructor);
Datum
chessboard_constructor(PG_FUNCTION_ARGS)
{
  char *fen = PG_GETARG_CSTRING(0);
  PG_RETURN_CHESSBOARD_P(FEN_to_chessboard(fen));
}

/******************************************************************************
 * Functions
******************************************************************************/

/**
  * @brief Returns the board state at a given half-move.
  * @param chessgame Pointer to the chessgame
  * @param number_half_moves Number of half-moves
  * @return *chessboard_t Pointer to the chessboard
  */
PG_FUNCTION_INFO_V1(getBoard); // Working
Datum
getBoard(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame = PG_GETARG_CHESSGAME_P(0);
  uint16_t number_half_moves = PG_GETARG_INT16(1);
  chessboard_t *chessboard = chessgame_to_chessboard(chessgame, number_half_moves);
  PG_FREE_IF_COPY(chessgame, 0);
  PG_RETURN_CHESSBOARD_P(chessboard);
}

/**
  * @brief Returns the chessgame truncated to its first N half-moves.
  * @param chessgame Pointer to the chessgame
  * @param number_half_moves Number of half-moves
  * @return *chessgame_t Pointer to the chessgame
  */
PG_FUNCTION_INFO_V1(getFirstMoves); // Working
Datum
getFirstMoves(PG_FUNCTION_ARGS) 
{
  chessgame_t *chessgame = PG_GETARG_CHESSGAME_P(0);
  uint16_t number_half_moves = PG_GETARG_INT16(1);
  chessgame_t *truncated_chessgame = truncate_chessgame(chessgame, number_half_moves);
  PG_FREE_IF_COPY(chessgame, 0);
  PG_RETURN_CHESSGAME_P(truncated_chessgame);
}

/**
  * @brief Returns true if the first chess game starts with the exact same set of moves as the second chess game.
  * The second parameter should not be a full game, but should only contain the opening moves that we want to check for, which can be of any length, i.e., m half-moves.
  * @param chessgame Pointer to the first chessgame
  * @param chessgame Pointer to the second chessgame
  * @return bool True if the first chessgame starts with the exact same set of moves as the second chessgame
  */
PG_FUNCTION_INFO_V1(hasOpening); // Working
Datum
hasOpening(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame_1 = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *chessgame_2 = PG_GETARG_CHESSGAME_P(1);
  bool hasOpening = strstr(chessgame_1->pgn, chessgame_2->pgn) != NULL;
  PG_FREE_IF_COPY(chessgame_1, 0);
  PG_FREE_IF_COPY(chessgame_2, 1);
  PG_RETURN_BOOL(hasOpening);
}

/**
  * @brief Returns true if the chessgame contains the given board state in its first N half-moves.
  * Only compare the state of the pieces and not compare the move count, castling right, en passant pieces, ..
  * @param chessgame Pointer to the chessgame
  * @param chessboard Pointer to the chessboard
  * @param number_half_moves Number of half-moves
  * @return bool True if the chessgame contains the given board state in its first N half-moves
*/
PG_FUNCTION_INFO_V1(hasBoard); // Working
Datum
hasBoard(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame = PG_GETARG_CHESSGAME_P(0);
  chessboard_t *chessboard = PG_GETARG_CHESSBOARD_P(1);
  uint16_t number_half_moves = PG_GETARG_INT16(2);
  chessboard_t *chessboard_of_chessgame = chessgame_to_chessboard(chessgame, number_half_moves); //one chessboard or list of chessboards?
  bool hasBoard = strcmp(chessboard_of_chessgame->fen, chessboard->fen); //Have to add function for only comparing the state of the pieces
  PG_FREE_IF_COPY(chessgame, 0);
  PG_FREE_IF_COPY(chessboard, 1);
  PG_RETURN_BOOL(hasBoard == 0);
}
