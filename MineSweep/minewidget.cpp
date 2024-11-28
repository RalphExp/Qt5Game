#include "minewidget.h"

#include <QPainter>

MineWidget::MineWidget(QWidget *parent) : QWidget(parent) {

}

void MineWidget::start(size_t width, size_t height, size_t mines) {
    width_ = width;
    height_ = height;
    mines_ = mines;

    board_ = vector<vector<bool>>(height, vector<bool>(width, 0));
    state_ = vector<vector<int>>(height, vector<int>(width, 0));
}

void MineWidget::paintEvent(QPaintEvent* e) {
    (void)e;
    QPainter painter(this);

    painter.drawRect(0, 0, width()-1, height()-1);
}
