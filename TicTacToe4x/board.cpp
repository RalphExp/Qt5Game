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
    srand(0);
}

Board::~Board() {

}

void Board::reset() {
    memset(board_, 0, sizeof(board_));
    win_path_.clear();
}

void Board::setWindow(MainWindow* win) {
    win_ = win;
    connect(this, SIGNAL(computerDone(int)), win_, SLOT(updateState(int)));
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
    b = (stat_ != kDraw &&
         stat_ != kNotStart &&
         player_ == kFirstPlay && first_play_) ||
    return b;
}

int Board::getScore(int b, int x, int y) {
    int pc = first_play_ ? OO : XX; // player color
    int cc = first_play_ ? XX : OO; // computer color

    int score = 0;
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
                score += 100;
            else if (pcCount == 2 && ccCount == 0)
                score += 6;
            else if (pcCount == 1 && ccCount == 0)
                score += 2;
            else if (ccCount == 1 && pcCount == 0)
                score += 1;
            else if (ccCount == 2 && pcCount == 0)
                score += 3;
            else if (ccCount == 3 && pcCount == 0)
                score += 1000;
        }
    }
    return score;
}

bool Board::isPlayerWin() {
    return ((stat_ == kFirstWin) && first_play_) ||
           ((stat_ == kSecondWin) && !first_play_);
}

bool Board::isComputerWin() {
    return ((stat_ == kFirstWin) && !first_play_) ||
           ((stat_ == kSecondWin) && first_play_);
}

int Board::minimax(int depth, bool maximizing,
        int alpha, int beta, int player, int computer) {

    if (isComputerWin()) {
        return 10 - depth;
    } else if (isPlayerWin()) {
        return depth - 10;
    } else if (steps_ == 64 || depth == 3) {
        return 0;
    }

    if (maximizing) {
        int best = 0x80000000;
        for (int b = 0; b < 4; ++b) {
            for (int x = 0; x < 4; ++x) {
                for (int y = 0; y < 4; ++y) {
                    if (board_[b][x][y] == NN) {
                        board_[b][x][y] = computer;
                        checkGameOver(b, x, y);
                        best = max(best, minimax(depth+1, false, alpha,
                            beta, player, computer));
                        --steps_;
                        board_[b][x][y] = NN;
                        alpha = max(alpha, best);
                        if (beta <= alpha) {
                            break;
                        }
                    }
                }
            }
        }
        return best;
    } else {
        int best = 0x7fffffff;
        for (int b = 0; b < 4; ++b) {
            for (int x = 0; x < 4; ++x) {
                for (int y = 0; y < 4; ++y) {
                    if (board_[b][x][y] == NN) {
                        board_[b][x][y] = player;
                        checkGameOver(b, x, y);
                        best = min(best, minimax(depth+1, true, alpha,
                            beta, player, computer));
                        --steps_;
                        board_[b][x][y] = NN;
                        beta = min(beta, best);
                        if (beta <= alpha) {
                            break;
                        }
                    }
                }
            }
        }
        return best;
    }
}

void Board::next(void) {
    int mb = 0;
    int mx = 0;
    int my = 0;
    int score = -1;

    int num = rand() % 64;
    int cc = first_play_ ? XX : OO;
    int pc = first_play_ ? OO : XX;

    for (int i = 0; i < 64; ++i) {
        int board = num / 16;
        int col = num % 4;
        int row = (num - 16 * board) / 4;

        if (board_[board][row][col] == NN) {
            int new_score = minimax(0, true, 0x80000000, 0x7fffffff, pc, cc);
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

    // sleep(1);
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
