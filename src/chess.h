#ifndef _CHESS_H
#define _CHESS_H

#define MAX_PGN_LENGTH 4096

/**
 * @brief Structure containing all information representing a chess game state using SAN notation.
 */
typedef struct {
  const char* pgn;
} chessgame_t;

/* fmgr macros chessgame type */
#define DatumGetChessgameP(X)  ((chessgame_t *) DatumGetPointer(X))
#define ChessgamePGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_CHESSGAME_P(n) DatumGetChessgameP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSGAME_P(x) return ChessgamePGetDatum(x)

/**
 * @brief Structure containing all information representing a chess board state using FEN notation.
 */
typedef struct {
  char *piece_placement_data;
  char active_color;
  char *castling_availability;
  char *en_passant_target_square;
  uint16_t halfmove_clock;
  uint16_t fullmove_clock;
} chessboard_t;

/* fmgr macros chessboard type */
#define DatumGetChessboardP(X)  ((chessboard_t *) DatumGetPointer(X))
#define ChessboardPGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_CHESSBOARD_P(n) DatumGetChessboardP(PG_GETARG_DATUM(n))
#define PG_RETURN_CHESSBOARD_P(x) return ChessboardPGetDatum(x)

static chessgame_t *chessgame_make(char *pgn);
static chessboard_t *chessboard_make(char *piece_placement_data, char active_color, char *castling_availability, char *en_passant_target_square, uint16_t halfmove_clock, uint16_t fullmove_clock);

static chessboard_t *chessgame_to_chessboard(chessgame_t *chessgame, uint16_t number_half_moves);
static void truncate_chessgame(char *truncated_pgn, char *pgn, uint16_t number_half_moves);
static chessgame_t *PGN_to_chessgame(char *pgn);
static chessboard_t *FEN_to_chessboard(char* fen);

#endif