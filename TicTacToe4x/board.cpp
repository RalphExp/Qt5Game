#include "board.h"

#include <memory.h>

Board::Board() : started_(false) {
    reset();
}

void Board::reset() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                board_[i][j][k] = N;
            }
        }
    }
}
