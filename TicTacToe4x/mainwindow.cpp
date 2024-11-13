#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QPainter>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);

    // pen
    pen_.setColor(Qt::black);
    pen_.setWidth(kPenSize);
    pen_.setCapStyle(Qt::RoundCap);
    pen_.setJoinStyle(Qt::RoundJoin);

    menu_height_ = menuBar()->height();
    setWindowTitle("TicTacToe X 4");

    connect(ui_->actionStart, &QAction::triggered,
        this, &MainWindow::startNewGame);
    connect(ui_->actionQuit, &QAction::triggered,
        this, &MainWindow::quitGame);

    setFixedSize(8+kGridSize*16, menu_height_+8+kGridSize*16);
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::startNewGame(void) {

}

void MainWindow::quitGame(void) {
    close();
}

void MainWindow::mousePressEvent(QMouseEvent* e) {
    int x = e->x();
    int y = e->y();
    int h = menu_height_;

    // adjust
    x -= 4;
    y -= 4+h;

    /* check if (x,y) in the grid */
    // cerr << "mouse clicked(" << x << "," << y << ")";

    if (x < 0 || y < 0 || x >= kPanelSize || y >= kPanelSize) {
        cerr << endl;
        return;
    } else if (x/kBoardSize != y/kBoardSize) {
        cerr << endl;
        return;
    }

    int b = x / kBoardSize;
    int r = (x-b*kBoardSize) / kGridSize;
    int c = (y-b*kBoardSize) / kGridSize;
    // cerr << "board:" << b << ", row:" << r << ", col:" << c << endl;

    board_.board_[size_t(r)][size_t(c)] = Board::O;
    update();
}

void MainWindow::drawCircle(QPainter& painter, int board, int row, int col) {

}

void MainWindow::drawCross(QPainter& painter, int board, int row, int col) {

}

void MainWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(pen_);

    int w = 4;
    int h = menu_height_ + 4;

    for (int b = 0; b < 4; ++b) {
        for (int y = 0; y <= 4; ++y) {
            for (int x = 0; x <= 4; ++x) {
                QLine horizontal(b*kBoardSize+w,
                    b*kBoardSize+y*kGridSize+h,
                    b*kBoardSize+kBoardSize+w,
                    b*kBoardSize+y*kGridSize+h);

                QLine vertical(b*kBoardSize+x*kGridSize+w,
                    b*kBoardSize+h,
                    b*kBoardSize+x*kGridSize+w,
                    b*kBoardSize+kBoardSize+h);
                painter.drawLine(horizontal);
                painter.drawLine(vertical);
            }
        }
    }

    for (int b = 0; b < 4; ++b) {
        for (int x = 0; x < 4; ++x) {
            for (int y = 0; y < 4; ++y) {
                switch (board_.board_[size_t(x)][size_t(y)]) {
                case Board::O:
                    drawCircle(painter, b, x, y);
                    break;
                case Board::X:
                    drawCross(painter, b, x, y);
                    break;
                default:
                    break;
                }
            }
        }
    }
}
