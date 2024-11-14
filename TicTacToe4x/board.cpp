#include "board.h"
#include "mainwindow.h"

#include <stdlib.h>
#include <memory.h>
#include <thread>

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
    connect(this, SIGNAL(computerDone(void)),
            win_, SLOT(update(void)));
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
        board_[b][x][y] = O;
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
    return 0;
}

bool Board::checkGameOver(int b, int x, int y) {
    return true;
}

int Board::setBoard(int b, int x, int y) {
    if (board_[b][x][y] != N)
        return 0;

    mtx_.lock();
    board_[b][x][y] = player_ == kFirstPlay ? O : X;
    player_ = !player_;
    mtx_.unlock();

    if (checkGameOver(b, x, y))
        return 2;

    future_ = std::async(std::launch::async, [this](){
        emit computerDone();
    });
    return 1;
}
