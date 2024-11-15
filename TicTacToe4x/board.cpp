#include "board.h"
#include "mainwindow.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <unistd.h>

#include <thread>
#include <vector>

Board::Board() {
    reset();
    srand(0);
}

Board::~Board() {

}

void Board::reset() {
    stat_ = kNotStart;
    steps_ = 0;
    memset(board_, 0, sizeof(board_));
    win_path_.clear();
}

void Board::setWindow(MainWindow* win) {
    win_ = win;
    connect(this, SIGNAL(computerDone(int)),
            win_, SLOT(updateState(int)));
}

void Board::start(bool firstPlay) {
    reset();
    stat_ = kPlaying;
    first_play_ = firstPlay;

    if (!firstPlay) {
        /* AI is firstplay */
        int b = rand() % 4;
        int x = rand() % 4;
        int y = rand() % 4;
        board_[b][x][y] = OO;
        ++steps_;
        player_ = kSecondPlay;
    } else {
        player_ = kFirstPlay;
    }
}

bool Board::isPlayerTurn(void) {
    bool b;
    mtx_.lock();
    b = (stat_ != kDraw && stat_ != kNotStart && player_ == kFirstPlay && first_play_) ||
        (player_ == kSecondPlay && !first_play_);
    mtx_.unlock();
    return b;
}

int Board::getScore(int b, int x, int y) {
    int pc = first_play_ ? OO : XX; // player color
    int cc = first_play_ ? XX : OO; // computer color

    int score = -1;
    vector<Path> paths = getPaths(b, x, y);
    for (auto& path : paths) {
        int pcCount = 0;
        int ccCount = 0;
        for (auto& g : path) {
            if (board_[g.b][g.x][g.y] == pc)
                ++pcCount;
            else if (board_[g.b][g.x][g.y] == cc)
                ++ccCount;
            if (pcCount == 3 && ccCount == 0)
                score += 1000;
            else if (pcCount == 2 && ccCount == 0)
                score += 4;
            else if (pcCount == 1 && ccCount == 0)
                score += 2;
            else if (ccCount == 1 && pcCount == 0)
                score += 1;
            else if (ccCount == 2 && pcCount == 0)
                score += 3;
            else if (ccCount == 3 && pcCount == 0)
                score += 10000;
        }
    }
    return score;
}

void Board::next(void) {
    int mb = 0;
    int mx = 0;
    int my = 0;
    int score = -1;

    int num = rand() % 64;
    for (int i = 0; i < 64; ++i) {
        int board = num / 16;
        int col = num % 4;
        int row = (num - 16 * board) / 4;

        if (board_[board][row][col] == NN) {
            int new_score = getScore(board, row, col);
            if (score < new_score) {
                mb = board;
                mx = row;
                my = col;
                score = new_score;
            }
        }
        ++num;
        num %= 64;
    }

    sleep(1);
    board_[mb][mx][my] = player_ == kFirstPlay ? OO : XX;
    checkGameOver(mb, mx, my);
    emit computerDone(stat_);
    player_ = !player_;
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
    ++steps_;
    vector<Path> v = getPaths(b, x, y);
    for (auto& p : v) {
        assert(p.size() == 4);
        bool over = true;
        for (auto &g : p) {
            auto mark = getBoard(g.b, g.x, g.y);
            if (mark == NN) {
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
        if (over) {
            if (player_ == kFirstPlay)
                stat_ = kFirstWin;
            else
                stat_ = kSecondWin;
            win_path_ = move(p);
            return;
        }
    }
    if (steps_ == 64) {
        stat_ = kDraw;
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
        future_ = std::async(std::launch::async, [this](){
            next();
        });
    }
    return stat_;
}
