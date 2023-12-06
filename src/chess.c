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
#include "utils/array.h"
#include "varatt.h"
#include "smallchesslib.h"
#include "chess.h"
PG_MODULE_MAGIC;

/******************************************************************************
 * Constructors for data types
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
 * Internal functions for data types
******************************************************************************/

/**
  * @brief Converts PGN to a chessgame
  * @param pgn PGN notation
  * @return *chessgame_t Pointer to the chessgame
*/
static chessgame_t *
PGN_to_chessgame(char *pgn)
{
  SCL_Record record;
  SCL_recordFromPGN(record, pgn);
  bool pgn_is_valid = SCL_recordLength(record) != 0;
  if (!pgn_is_valid)
  {
    ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid input syntax for type %s: \"%s\"", "chessgame", pgn)));
  }
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
FEN_to_chessboard(char *fen) //this function should handle errors
{
  SCL_Board board;
  bool fen_is_valid = SCL_boardFromFEN(board, fen);
  if (!fen_is_valid)
  {
    ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION), errmsg("invalid input syntax for type %s: \"%s\"", "chessboard", fen)));
  }
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

/**
 * @brief Converts a chessgame to a chessboard at a given half-move.
 * @param chessgame Pointer to the chessgame
 * @param number_half_moves Number of half-moves
 * @return *chessboard_t Pointer to the chessboard
*/
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
  * @brief Truncates a chessgame to its first N half-moves.
  * @param chessgame Pointer to the chessgame
  * @param number_half_moves Number of half-moves
  * @return *chessgame_t Pointer to the new truncated chessgame
*/
chessgame_t * 
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

/**
 * @brief Compares the moves of two chessgames.
 * @param chessgame_1 Pointer to the first chessgame
 * @param chessgame_2 Pointer to the second chessgame
 * @return bool True if the first chessgame starts with the exact same set of moves as the second chessgame
*/
static bool 
compare_moves(chessgame_t *chessgame_1, chessgame_t *chessgame_2)
{
  return strstr(chessgame_1->pgn, chessgame_2->pgn) != NULL;
}

/**
 * @brief Compares the pieces of two chessboards.
 * @param chessboard_1 Pointer to the first chessboard
 * @param chessboard_2 Pointer to the second chessboard
 * @return bool True if the first chessboard contains the same pieces as the second chessboard
*/
static bool
compare_pieces(chessboard_t *chessboard_1, chessboard_t *chessboard_2)
{
  char *pieces_1 = (char *) malloc(sizeof(char) * SCL_FEN_MAX_LENGTH);
  char *pieces_2 = (char *) malloc(sizeof(char) * SCL_FEN_MAX_LENGTH);
  uint16_t i = 0;
  uint16_t j = 0;
  while (chessboard_1->fen[i] != ' ' && chessboard_1->fen[i] != '\0') {
    pieces_1[i] = chessboard_1->fen[i];
    i += 1;
  }
  pieces_1[i] = '\0';

  while (chessboard_2->fen[j] != ' ' && chessboard_2->fen[j] != '\0') {
    pieces_2[j] = chessboard_2->fen[j];
    j += 1;
  }
  pieces_2[j] = '\0';

  bool result = strcmp(pieces_1, pieces_2) == 0;
  free(pieces_1);
  free(pieces_2); 
  return result;
}

/**
 * @brief Returns true if the chessgame contains the given board state in its first N half-moves.
 * Only compare the state of the pieces and not compare the move count, castling right, en passant pieces, etc.
 * @param chessgame Pointer to the chessgame
 * @param chessboard Pointer to the chessboard
 * @param number_half_moves Number of half-moves
 * @return bool True if the chessgame contains the given board state in its first N half-moves
*/
static bool
chessgame_contains_chessboard(chessgame_t *chessgame, chessboard_t *chessboard, uint16_t number_half_moves)
{
  for (uint16_t i = 0; i <= number_half_moves; i++) {
    if (compare_pieces(chessgame_to_chessboard(chessgame, i), chessboard)) {
      return true;
    }
  }
  return false;
}

int
chessgame_to_number(chessgame_t *chessgame)
{
  SCL_Record record;
  SCL_recordInit(record);
  SCL_recordFromPGN(record, chessgame->pgn);
  return SCL_recordLength(record) + 1; //car on compte le 0
}

static ArrayType *
chessgame_to_chessboards_internal(chessgame_t *chessgame)
{
  int number_half_moves = chessgame_to_number(chessgame);
  chessboard_t **chessboards = (chessboard_t **) palloc(sizeof(chessboard_t *) * number_half_moves);

  for (int i = 0; i < number_half_moves; i++) {
    chessboards[i] = chessgame_to_chessboard(chessgame, i);
  }

  Oid chessboard_oid = TypenameGetTypid("chessboard");
  return construct_array((Datum *) chessboards, number_half_moves, chessboard_oid, -1, false, 'i');
}

/******************************************************************************
 * Input/output for chessgame data type
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
 * Input/output functions for chessboard data type
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
 * Constructors for data types
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
 * Function for indexing
******************************************************************************/

PG_FUNCTION_INFO_V1(chessgame_to_chessboards);

Datum
chessgame_to_chessboards(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame = PG_GETARG_CHESSGAME_P(0);
  ArrayType *array;
  SCL_Record record;
  SCL_recordInit(record);
  SCL_recordFromPGN(record, chessgame->pgn);
  int number_half_moves = SCL_recordLength(record) + 1; //car on compte le 0

  chessboard_t **chessboards = (chessboard_t **) palloc(sizeof(chessboard_t *) * number_half_moves);

  for (int i = 0; i < number_half_moves; i++) {
    chessboards[i] = chessgame_to_chessboard(chessgame, i);
  }

  Oid chessboard_oid = TypenameGetTypid("chessboard");
  array = construct_array((Datum *) chessboards, number_half_moves, chessboard_oid, -1, false, 'i');
  PG_FREE_IF_COPY(chessgame, 0);
  PG_RETURN_ARRAYTYPE_P(array);
}

/******************************************************************************
 * Functions and predicates
******************************************************************************/

/**
  * @brief Returns the board state at a given half-move.
  * @param chessgame Pointer to the chessgame
  * @param number_half_moves Number of half-moves
  * @return *chessboard_t Pointer to the chessboard
*/
PG_FUNCTION_INFO_V1(getBoard);
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
PG_FUNCTION_INFO_V1(getFirstMoves);
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

/**
  * @brief Returns true if the chessgame contains the given board state in its first N half-moves.
  * Only compare the state of the pieces and not compare the move count, castling right, en passant pieces, etc.
  * @param chessgame Pointer to the chessgame
  * @param chessboard Pointer to the chessboard
  * @param number_half_moves Number of half-moves
  * @return bool True if the chessgame contains the given board state in its first N half-moves
*/
PG_FUNCTION_INFO_V1(hasBoard);
Datum
hasBoard(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame = PG_GETARG_CHESSGAME_P(0);
  chessboard_t *chessboard = PG_GETARG_CHESSBOARD_P(1);
  uint16_t number_half_moves = PG_GETARG_INT16(2);
  bool hasBoard = chessgame_contains_chessboard(chessgame, chessboard, number_half_moves);
  PG_FREE_IF_COPY(chessgame, 0);
  PG_FREE_IF_COPY(chessboard, 1);
  PG_RETURN_BOOL(hasBoard);
}
