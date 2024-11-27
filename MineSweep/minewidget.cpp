#include "minewidget.h"

#include <QPainter>

MineWidget::MineWidget(QWidget *parent) : QWidget(parent) {

}

void MineWidget::paintEvent(QPaintEvent* e) {
    (void)e;
    QPainter painter(this);

    painter.drawRect(0, 0, width()-1, height()-1);
}
