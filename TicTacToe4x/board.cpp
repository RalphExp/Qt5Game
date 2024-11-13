#include "board.h"

Board::Board() : started_(false) {
    board_ = vector<vector<int>>(4, vector<int>(4, Mark::N));
}
