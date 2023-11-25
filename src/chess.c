/*
 * chess.c
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
#include "smallchesslib.h"
#include "chess.h"

PG_MODULE_MAGIC;

/*****************************************************************************/

/* Constructors */

/**
 * @brief Constructs a chessgame
 * @param pgn PGN notation of the chessgame
 * @return *chessgame_t Pointer to the chessgame
 */
static chessgame_t
*chessgame_make(const char *pgn)
{
  chessgame_t *chessgame = palloc0(sizeof(chessgame_t));
  chessgame->pgn = (char *)malloc(sizeof(char) * strlen(pgn) + 1);
  strcpy(chessgame->pgn, pgn);
  return chessgame;
}


/**
 * @brief Constructs a chessboard
 * @param piece_placement_data Piece placement data
 * @param active_color Active color
 * @param castling_availability Castling availability
 * @param en_passant_target_square En passant target square
 * @param halfmove_clock Halfmove clock
 * @param fullmove_clock Fullmove clock
 * @return *chessboard_t Pointer to the chessboard
 */
static chessboard_t 
*chessboard_make(char *piece_placement_data, char active_color, char *castling_availability, char *en_passant_target_square, uint16_t halfmove_clock, uint16_t fullmove_clock)
{
  chessboard_t *chessboard = palloc0(sizeof(chessboard_t));
  chessboard->piece_placement_data = (char *)malloc(sizeof(char) * strlen(piece_placement_data) + 1);
  strcpy(chessboard->piece_placement_data, piece_placement_data);
  chessboard->active_color = active_color;
  chessboard->castling_availability = (char *)malloc(sizeof(char) * strlen(castling_availability) + 1);
  strcpy(chessboard->castling_availability, castling_availability);
  chessboard->en_passant_target_square = (char *)malloc(sizeof(char) * strlen(en_passant_target_square) + 1);
  strcpy(chessboard->en_passant_target_square, en_passant_target_square);
  chessboard->halfmove_clock = halfmove_clock;
  chessboard->fullmove_clock = fullmove_clock;
  return chessboard;
}

/*****************************************************************************/

/* Functions for data types */

/**
  * @brief Truncates the chessgame to its first N half-moves.
  * @param *truncated_pgn Pointer to the truncated PGN notation
  * @param *pgn Pointer to the PGN notation
  * @param number_half_moves Number of half-moves
  */
*/
static void 
truncate_chessgame(char *truncated_pgn, char *pgn, uint16_t number_half_moves)
{
  char delimeter = ' ';
  uint16_t count = 0;
  while(count < number_half_moves) {
    if(pgn[count] == delimeter) {
      count += 1;
    }
    truncated_pgn[count] = pgn[count];
    count += 1;
  }
  truncated_pgn[count] = '\0';
  return truncated_pgn;
}

/**
  * @brief Converts PGN to a chessgame
  * @param pgn PGN notation
  * @return *chessgame_t Pointer to the chessgame
  */
static chessgame_t *
PGN_to_chessgame(char *pgn)
{
  return chessgame_make(pgn);
}

/**
  * @brief Converts FEN to a chessboard
  * @param fen FEN notation
  * @return *chessboard_t Pointer to the chessboard
  */
static chessboard_t *
FEN_to_chessboard(char* fen) 
{
  char *token = strtok(fen, " ");
  char *list[6];
  int index = 0;

  while(token != NULL) {
    list[index] = token;
    token = strtok(NULL, " ");
    index += 1;
  }

  return chessboard_make(list[0], *list[1], list[2], list[3], atoi(list[4]), atoi(list[5]));
}

/*****************************************************************************/

/* Functions */

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
  SCL_Record record;
  SCL_Board board;
  char *fen = (char *)malloc(sizeof(char) * SCL_FEN_MAX_LENGTH);
  SCL_recordFromPGN(record, chessgame->pgn);
  SCL_recordApply(record, board, number_half_moves);
  SCL_boardToFEN(board, fen);
  chessboard_t *chessboard = FEN_to_chessboard(fen);
  free(fen);
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
{ // Another possibilty is to implement function that truncate chessgame to n first moves -> new chessgame
  chessgame_t *chessgame = PG_GETARG_CHESSGAME_P(0);
  uint16_t number_half_moves = PG_GETARG_INT16(1);
  char *truncated_pgn = (char *)malloc(sizeof(char) * MAX_PGN_LENGTH);
  truncate_chessgame(truncated_pgn, chessgame->pgn, number_half_moves);
  chessgame_t *new_chessgame = PGN_to_chessgame(truncated_pgn); // new chessgame or chessgame ?
  PG_FREE_IF_COPY(chessgame, 0);
  free(truncated_pgn);
  PG_RETURN_CHESSGAME_P(new_chessgame)
}

/**
  * @brief Returns true if the first chess game starts with the exact same set of moves as the second chess game.
  * The second parameter should not be a full game, but should only contain the opening moves that we want to check for, which can be of any length, i.e., m half-moves.
  * @param chessgame Pointer to the first chessgame
  * @param chessgame Pointer to the second chessgame
  * @return bool True if the first chessgame starts with the exact same set of moves as the second chessgame
  */
PG_FUNCTION_INFO_V1(hasOpening);
Datum
hasOpening(PG_FUNCTION_ARGS)
{
  chessgame_t *chessgame_1 = PG_GETARG_CHESSGAME_P(0);
  chessgame_t *chessgame_2 = PG_GETARG_CHESSGAME_P(1);
  bool hasOpening = strcmp(chessgame_1->pgn, chessgame_2->pgn);
  PG_FREE_IF_COPY(chessgame_1, 0);
  PG_FREE_IF_COPY(chessgame_2, 1);
  PG_RETURN_BOOL(hasOpening == 0);
}

/**
  * @brief  Returns true if the chessgame contains the given board state in its first N half-moves.
  * Only compare the state of the pieces and not compare the move count, castling right, en passant pieces, ..
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
  chessboard_t *chessboard_of_chessgame = getBoard(chessgame, number_half_moves);
  bool hasBoard = strcmp(chessboard_of_chessgame->piece_placement_data, chessboard->piece_placement_data);
  PG_FREE_IF_COPY(chessgame, 0);
  PG_FREE_IF_COPY(chessboard, 1);
  PG_RETURN_BOOL(hasBoard == 0);
}
