#include "../src/chess.h"

int main() {
    chessboard_t *chessboard = FEN_parsing(STARTING_POSITION);
    print_chessboard(chessboard);
}