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
    switch (state_[size_t(y)][size_t(x)]) {
    case kNormal:
        painter.setBrush(Qt::gray);
        painter.drawRoundRect(x*gsize_+2, y*gsize_+2, gsize_-2, gsize_-2, 16, 16);
        break;
    case kPressed:
        painter.setBrush(Qt::lightGray);
        painter.drawRoundRect(x*gsize_+2, y*gsize_+2, gsize_-2, gsize_-2, 16, 16);
        break;
    }
}

void MineWidget::paintEvent(QPaintEvent* e) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    QRect rc = e->rect();
    qDebug() << "rect" << rc;

    // TODO: optimized update

    for (int y = 0; y < static_cast<int>(board_.size()); ++y) {
        for (int x = 0; x < static_cast<int>(board_[0].size()); ++x) {
            drawGrid(painter, x, y);
        }
    }
}

void MineWidget::leaveEvent(QEvent*) {
    // qDebug() << "leave";

    if (mouseX_ >= 0 && mouseX_ < width_ && mouseY_ >= 0 && mouseY_ < height_) {
        state_[size_t(mouseY_)][size_t(mouseX_)] = kNormal;
    }
    mouseX_ = -1;
    mouseY_ = -1;
    update();
}

void MineWidget::mouseMoveEvent(QMouseEvent* event) {
    int gx = event->x() / gsize_;
    int gy = event->y() / gsize_;

    if (gx == mouseX_ && gy == mouseY_)
        return;

    // restore old grid
    if (mouseX_ >= 0 && mouseX_ < width_ && mouseY_ >= 0 && mouseY_ < height_) {
        state_[size_t(mouseY_)][size_t(mouseX_)] = kNormal;
        update(QRect(mouseX_*gsize_, mouseY_*gsize_, gsize_, gsize_));
    }

    // draw new grid
    if (gx >= 0 && gx < width_ && gy >= 0 && gy < height_) {
        state_[size_t(gy)][size_t(gx)] = kPressed;
        update(QRect(gx*gsize_, gy*gsize_, gsize_, gsize_));
    }
    mouseX_ = gx;
    mouseY_ = gy;
}



