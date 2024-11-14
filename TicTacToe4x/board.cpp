#include "board.h"
#include "mainwindow.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <unistd.h>

#include <thread>
#include <vector>

Board::Board() : stat_(kNotStart), player_(kFirstPlay) {
    reset();
}

Board::~Board() {

}

void Board::reset() {
    memset(board_, 0, sizeof(board_));
}

void Board::setWindow(MainWindow* win) {
    win_ = win;
    connect(this, SIGNAL(computerDone(int)),
            win_, SLOT(updateState(int)));
}

void Board::start(bool firstPlay) {
    stat_ = kPlaying;
    first_play_ = firstPlay;
    reset();

    if (!firstPlay) {
        /* AI is firstplay */
        int b = rand() % 4;
        int x = rand() % 4;
        int y = rand() % 4;
        board_[b][x][y] = OO;
        player_ = kSecondPlay;
    } else {
        player_ = kFirstPlay;
    }
}

bool Board::isPlayerTurn(void) {
    bool b;
    mtx_.lock();
    b = (player_ == kFirstPlay && first_play_) ||
        (player_ == kSecondPlay && !first_play_);
    mtx_.unlock();
    return b;
}

int Board::next(int b, int x, int y) {
    int mb = 0;
    int mx = 0;
    int my = 0;

    for (int a = 0; a < 4; ++a) {
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                if (board_[a][r][c] == NN) {
                    mb = a;
                    mx = 0;
                    my = 0;
                    goto done;
                }
            }
        }
    }
done:
    sleep(1);
    board_[mb][mx][my] = player_ == kFirstPlay ? OO : XX;
    checkGameOver(b, x, y);
    player_ = !player_;
    emit computerDone(stat_);
    return 0;
}

vector<Path> Board::getPaths(int b, int x, int y) {
    vector<Path> v;
    Path path;

    /* 4d */
    path = Path{
        {0, x, y},
        {1, x, y},
        {2, x, y},
        {3, x, y}
    };
    v.push_back(path);

    /* vertical */
    path = Path{
        {b, 0, y},
        {b, 1, y},
        {b, 2, y},
        {b, 3, y}
    };
    v.push_back(path);

    /* horizontal */
    path = Path{
        {b, x, 0},
        {b, x, 1},
        {b, x, 2},
        {b, x, 3}
    };
    v.push_back(path);

    /* diagonal */
    if (x == y) {
        path = Path{
            {b, 0, 0},
            {b, 1, 1},
            {b, 2, 2},
            {b, 3, 3}
        };
        v.push_back(path);

        /* 4d diagonal */
        if (b == x && b == y) {
            path = Path {
                {0, 0, 0},
                {1, 1, 1},
                {2, 2, 2},
                {3, 3, 3}
            };
            v.push_back(path);
        } else if (b + x == 3) {
            path = Path {
                {0, 3, 3},
                {1, 2, 2},
                {2, 1, 1},
                {3, 0, 0}
            };
            v.push_back(path);
        }
    }

    /* back diagonal */
    if (x + y == 3) {
        path = Path{
            {b, 0, 3},
            {b, 1, 2},
            {b, 2, 1},
            {b, 3, 0}
        };
        v.push_back(path);

        /* 4d back diagonal */
        if (b == x) {
            path = Path{
                {0, 0, 3},
                {1, 1, 2},
                {2, 2, 1},
                {3, 3, 0}
            };
            v.push_back(path);
        } else if (b == y) {
            path = Path{
                {0, 3, 0},
                {1, 2, 1},
                {2, 1, 2},
                {3, 0, 3},
            };
            v.push_back(path);
        }
    }

    /* 4d vertical */
    if (b == x) {
        path = Path{
            {0, 0, y},
            {1, 1, y},
            {2, 2, y},
            {3, 3, y}
        };
        v.push_back(path);
    } else if (b + x == 3) {
        path = Path{
            {0, 3, y},
            {1, 2, y},
            {2, 1, y},
            {3, 0, y}
        };
        v.push_back(path);
    }

    /* 4d horizontal */
    if (b == y) {
        path = Path {
            {0, x, 0},
            {1, x, 1},
            {2, x, 2},
            {3, x, 3}
        };
        v.push_back(path);
    } else if (b + y == 3) {
        path = Path {
            {0, x, 3},
            {1, x, 2},
            {2, x, 3},
            {3, x, 1}
        };
        v.push_back(path);
    }
    return v;
}

int Board::operator[](const Grid& g) {
    return getBoard(g.b, g.x, g.y);
}

int Board::getBoard(int b, int x, int y) {
    return board_[b][x][y];
}

void Board::checkGameOver(int b, int x, int y) {
    vector<Path> v = getPaths(b, x, y);
    for (auto& p : v) {
        assert(p.size() == 4);
        bool over = true;
        for (auto &g : p) {
            auto mark = getBoard(g.b, g.x, g.y);
            if (mark == Board::NN) {
                over = false;
                break;
            } else if (mark == OO && player_ == kSecondPlay) {
                over = false;
                break;
            } else if (mark == XX && player_ == kFirstPlay) {
                over = false;
                break;
            }
        }
        if (over == true) {
            if (player_ == kFirstPlay)
                stat_ = kFirstWin;
            else
                stat_ = kSecondWin;
            return;
        }
    }
}

int Board::setBoard(int b, int x, int y) {
    std::lock_guard guard(mtx_);

    if (board_[b][x][y] != NN)
        return -1;

    board_[b][x][y] = player_ == kFirstPlay ? OO : XX;
    checkGameOver(b, x, y);

    player_ = !player_;
    if (stat_ < kFirstWin) {
        future_ = std::async(std::launch::async, [this, b, x, y](){
            next(b, x, y);
        });
    }
    return stat_;
}
