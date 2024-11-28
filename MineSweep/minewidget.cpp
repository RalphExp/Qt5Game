#include "minewidget.h"

#include <QPainter>
#include <QLayout>
#include <QDebug>

MineWidget::MineWidget(QWidget *parent) : QWidget(parent),
    gsize_(32), mouseX_(-1), mouseY_(-1) {

    setMouseTracking(true);
}

void MineWidget::start(int width, int height, int mines) {
    width_ = width;
    height_ = height;
    mines_ = mines;

    board_ = vector<vector<bool>>(
        static_cast<size_t>(height),
        vector<bool>(static_cast<size_t>(width), 0));

    state_ = vector<vector<int>>(
        static_cast<size_t>(height),
        vector<int>(static_cast<size_t>(width), 0));

    resize(width_*gsize_, height_*gsize_);
}

void MineWidget::drawGrid(QPainter& painter, int x, int y) {
    switch (state_[x][y]) {
    case kHide:
        painter.setBrush(Qt::gray);
        painter.drawRoundRect(x*gsize_+2, y*gsize_+2, gsize_-2, gsize_-2, 16, 16);
        break;
    case kBlank:
        painter.setBrush(Qt::lightGray);
        painter.drawRoundRect(x*gsize_+2, y*gsize_+2, gsize_-2, gsize_-2, 16, 16);
        break;
    }
}

void MineWidget::paintEvent(QPaintEvent* e) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    for (int x = 0; x < static_cast<int>(board_.size()); ++x) {
        for (int y = 0; y < static_cast<int>(board_[0].size()); ++y) {
            drawGrid(painter, x, y);
        }
    }
}

void MineWidget::mouseMoveEvent(QMouseEvent* event) {
    qDebug() << "Mouse Mouve";

    if (mouseX_ != event->x() || mouseY_ != event->y()) {
        int x = mouseX_ / gsize_;
        int y = mouseY_ / gsize_;
        if (x >= 0 && x < width_ && y >= 0 && y < height_) {
            state_[x][y] = kHide;
        }
    }

    mouseX_ = event->x();
    mouseY_ = event->y();
    int x = mouseX_ / gsize_;
    int y = mouseY_ / gsize_;
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        state_[x][y] = kBlank;
    }
    update();
}



