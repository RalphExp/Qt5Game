#ifndef BOARD_H
#define BOARD_H

#include <vector>

using namespace std;

class Board {
public:
    Board();
    enum {
        N = 0,
        O = 1,
        X = 2
    };

public:
    vector<vector<int>> board_;
    bool started_;
};

#endif // BOARD_H
