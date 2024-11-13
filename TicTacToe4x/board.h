#ifndef BOARD_H
#define BOARD_H

#include <vector>

using namespace std;

class Board {
public:
    Board();
    void reset();

    enum {
        N = 0,
        O = 1,
        X = 2
    };

public:
    int board_[4][4][4];
    bool started_;
};

#endif // BOARD_H
