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
#include <stdio.h>
#include "utils/builtins.h"
#include "libpq/pqformat.h"
#include "chess.h"

PG_MODULE_MAGIC;

/**
 * @brief Constructs a chessboard
 * @param piece_placement_data Piece placement data
 * @param active_color
 * @param castling_availability
 * @param en_passant_target_square
 * @param halfmove_clock
 * @param fullmove_clock
 */
static chessboard_t * chessboard_make(char *piece_placement_data, char active_color, char *castling_availability, char *en_passant_target_square, int halfmove_clock, int fullmove_clock) {
  chessboard_t *chessboard = (chessboard_t *)malloc(sizeof(chessboard_t));
  strcpy(chessboard->piece_placement_data, piece_placement_data);
  chessboard->active_color = active_color;
  strcpy(chessboard->castling_availability, castling_availability);
  strcpy(chessboard->en_passant_target_square, en_passant_target_square);
  chessboard->halfmove_clock = halfmove_clock;
  chessboard->fullmove_clock = fullmove_clock;
  return chessboard;
}

static chessboard_t * FEN_parsing(char* fen) {
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

void print_chessboard(chessboard_t *chessboard) {
  printf("Piece placement data : %s", chessboard->piece_placement_data);
  printf("Active color : %d", chessboard->active_color);
  printf("Castling availability : %s", chessboard->castling_availability);
  printf("En passant target square : %s", chessboard->en_passant_target_square);
  printf("Halfmove clock : %d", chessboard->halfmove_clock);
  printf("Fullmove clock: %d", chessboard->fullmove_clock);
} 
 
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
