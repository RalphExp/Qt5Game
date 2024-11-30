#include "minewidget.h"

#include <QPainter>
#include <QLayout>
#include <QDebug>

MineWidget::MineWidget(QWidget *parent) : QWidget(parent),
    gsize_(32), mouseX_(-1), mouseY_(-1), state_(),
    gameState_(kNotStarted) {

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

void MineWidget::drawNormal(QPainter& painter, int x, int y) {
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.drawRoundRect(x*gsize_+4, y*gsize_+4, gsize_-4, gsize_-4, 16, 16);
}

void MineWidget::drawPressed(QPainter& painter, int x, int y) {
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::lightGray);
    painter.drawRoundRect(x*gsize_+4, y*gsize_+4, gsize_-4, gsize_-4, 16, 16);
}

void MineWidget::drawFlag(QPainter& painter, int x, int y) {

}

void MineWidget::drawQuestion(QPainter& painter, int x, int y) {

}

void MineWidget::drawNumber(QPainter& painter, int x, int y, int n) {

}

void MineWidget::drawGrid(QPainter& painter, int x, int y) {
    switch (state_[size_t(y)][size_t(x)]) {
    case kNormal:
        drawNormal(painter, x, y);
        break;
    case kPressed:
        drawPressed(painter, x, y);
        break;
    case kFlag:
        break;
    case kMine:
        break;
    case kExploded:
        break;
    case kNumber:   // fall through
    case kNumber+1: // fall through
    case kNumber+2: // fall through
    case kNumber+3: // fall through
    case kNumber+4: // fall through
    case kNumber+5: // fall through
    case kNumber+6: // fall through
    case kNumber+7:
        drawNumber(painter, x, y, state_[size_t(y)][size_t(x)]-kNumber+1);
        break;
    default:
        break;
    }
}

void MineWidget::paintEvent(QPaintEvent* e) {
    QPainter painter(this);
    QRect rc = e->rect();

    // qDebug() << "rect" << rc;

    int bx = rc.x() / gsize_;
    int by = rc.y() / gsize_;
    int ex = (rc.x() + rc.width()) / gsize_;
    int ey = (rc.y() + rc.height()) / gsize_;

    bx = min(max(0, bx), width_-1);
    ex = min(max(0, ex), width_-1);

    by = min(max(0, by), height_-1);
    ey = min(max(0, ey), height_-1);

    for (int y = by; y < ey+1; ++y) {
        for (int x = bx; x < ex+1; ++x) {
            drawGrid(painter, x, y);
        }
    }
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

    // note that the update rect will be combined.
    mouseX_ = gx;
    mouseY_ = gy;
}

void MineWidget::mousePressEvent(QMouseEvent* event) {
    int gx = event->x() / gsize_;
    int gy = event->y() / gsize_;

    if (gx < 0 || gx >= width_ || gy < 0 || gy > height_)
        return;

    if (event->button() & Qt::MouseButton::RightButton) {
        if (gameState_ != kStarted)
            return;

        bool needUpdate = true;
        switch (state_[size_t(gy)][size_t(gx)]) {
        case kNormal:
            state_[size_t(gy)][size_t(gx)] = kFlag;
            break;
        case kFlag:
            state_[size_t(gy)][size_t(gx)] = kQuestion;
            break;
        case kQuestion:
            state_[size_t(gy)][size_t(gx)] = kNormal;
            break;
        default:
            needUpdate = false;
            break;
        }
        if (needUpdate) {
            update(QRect(gx*gsize_, gy*gsize_, gsize_, gsize_));
        }
    } else {

    }
}

void MineWidget::leaveEvent(QEvent*) {
    if (mouseX_ >= 0 && mouseX_ < width_ && mouseY_ >= 0 && mouseY_ < height_) {
        state_[size_t(mouseY_)][size_t(mouseX_)] = kNormal;
    }

    update(QRect(mouseX_*gsize_, mouseY_*gsize_, gsize_, gsize_));
    mouseX_ = -1;
    mouseY_ = -1;
}
