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
    QRect rect(x*gsize_+4, y*gsize_+4, gsize_-4, gsize_-4);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundRect(rect, 16, 16);

    painter.setBrush(Qt::red);
    QPoint points[3] = {
        QPoint(x*gsize_+4+(gsize_-4)*4/16, y*gsize_+4+(gsize_-4)*3/16),
        QPoint(x*gsize_+4+(gsize_-4)*13/16, y*gsize_+4+(gsize_-4)*6/16),
        QPoint(x*gsize_+4+(gsize_-4)*4/16, y*gsize_+4+(gsize_-4)*9/16)
    };
    painter.drawPolygon(points, 3);

    /* draw pole*/
    QPen pen;
    pen.setColor(QColor("#000000"));
    pen.setWidth(gsize_/10);
    painter.setPen(pen);
    painter.setBrush(Qt::red);
    painter.drawLine(x*gsize_+4+(gsize_-4)*4/16,
                     y*gsize_+4+(gsize_-4)*3/16,
                     x*gsize_+4+(gsize_-4)*4/16,
                     y*gsize_+4+(gsize_-4)*13/16);
}

void MineWidget::drawQuestion(QPainter& painter, int x, int y) {
    drawFlag(painter, x, y);
    QRect rect(x*gsize_+gsize_/2, y*gsize_+gsize_/2, gsize_/2, gsize_/2);
    QFont font = painter.font();
    font.setPointSize(gsize_/4);
    font.setBold(true);

    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(rect, Qt::AlignCenter, QString("?"));
}

void MineWidget::drawNumber(QPainter& painter, int x, int y, int n) {
    static QColor colors[8] = {QColor("#e0ffe0"),};

    QRect rect(x*gsize_+4, y*gsize_+4, gsize_-4, gsize_-4);
    QFont font = painter.font();
    font.setPointSize(gsize_/2);
    font.setBold(true);

    painter.setFont(font);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(colors[n-1]));
    painter.drawRoundRect(rect, 16, 16);
    painter.setPen(Qt::black);
    painter.drawText(rect, Qt::AlignCenter, QString('0'+n));
}

void MineWidget::drawGrid(QPainter& painter, int x, int y) {
    qDebug() << "(" << x << "," << y << ") state: " << state_[size_t(y)][size_t(x)];

    switch (state_[size_t(y)][size_t(x)]) {
    case kNormal:
        drawNormal(painter, x, y);
        break;
    case kPressed:
        drawPressed(painter, x, y);
        break;
    case kFlag:
        drawFlag(painter, x, y);
        break;
    case kQuestion:
        drawQuestion(painter, x, y);
        break;
    case kMine:
        break;
    case kExploded:
        break;
    case kNumber:   // fall through to kNumber+7
    case kNumber+1:
    case kNumber+2:
    case kNumber+3:
    case kNumber+4:
    case kNumber+5:
    case kNumber+6:
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
        if (state_[size_t(mouseY_)][size_t(mouseX_)] == kPressed) {
            state_[size_t(mouseY_)][size_t(mouseX_)] = kNormal;
            update(QRect(mouseX_*gsize_, mouseY_*gsize_, gsize_, gsize_));
        }
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

    if (event->button() & Qt::RightButton) {
        // if (gameState_ != kStarted)
        //     return;
        qDebug() << "MouseEvent: " << event->button();

        bool needUpdate = true;
        switch (state_[size_t(gy)][size_t(gx)]) {
        case kNormal: // fall through
        case kPressed:
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
            qDebug() << "update mouse click";
            update(QRect(gx*gsize_, gy*gsize_, gsize_, gsize_));
        }
    } else {
        state_[size_t(gy)][size_t(gx)] = kNumber;
        update(QRect(gx*gsize_, gy*gsize_, gsize_, gsize_));
    }
}

void MineWidget::leaveEvent(QEvent*) {
    if (mouseX_ >= 0 && mouseX_ < width_ && mouseY_ >= 0 && mouseY_ < height_) {
        if (state_[size_t(mouseY_)][size_t(mouseX_)] == kPressed) {
            state_[size_t(mouseY_)][size_t(mouseX_)] = kNormal;
            update(QRect(mouseX_*gsize_, mouseY_*gsize_, gsize_, gsize_));
        }
    }
    mouseX_ = -1;
    mouseY_ = -1;
}
