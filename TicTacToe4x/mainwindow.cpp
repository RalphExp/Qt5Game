#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QPainter>
#include <QBrush>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);

    board_.setWindow(this);
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
    auto res = QMessageBox::information(this, "TicTacToeX4", "Go first?",
            QMessageBox::Yes, QMessageBox::No);

    board_.start(res == QMessageBox::Yes);
    update();
}

void MainWindow::quitGame(void) {
    close();
}

void MainWindow::showBoardStat(void) {
    cerr << "State: " << board_.getState() << "|"
         << "FirstPlay: " << boolalpha << board_.isFirstPlay() << "|"
         << "Play's turn: " << board_.isPlayerTurn() << endl;
}

void MainWindow::handleMousePress(int x, int y) {
    if (board_.getState() >= Board::kFirstWin || !board_.isPlayerTurn())
        return;

    int h = menu_height_;

    // adjust
    x -= 4;
    y -= 4+h;

    if (x < 0 || y < 0 || x >= kPanelSize || y >= kPanelSize) {
        return;
    } else if (x/kBoardSize != y/kBoardSize) {
        return;
    }

    int b = x / kBoardSize;
    int r = (x-b*kBoardSize) / kGridSize;
    int c = (y-b*kBoardSize) / kGridSize;

    int stat = board_.setBoard(b, r, c);
    updateState(stat);
}

void MainWindow::mousePressEvent(QMouseEvent* e) {
    if (e->button() & Qt::LeftButton) {
        return handleMousePress(e->x(), e->y());
    } else if (e->button() & Qt::RightButton) {
        return showBoardStat();
    }
}

void MainWindow::drawGrid(QPainter& painter, int b, int x, int y, int c) {
    auto h = menu_height_;
    Grid g{b, x, y};
    auto path = board_.getWinPath();
    if (std::find(path.begin(), path.end(), g) != path.end()) {
        painter.setPen(QPen(Qt::magenta, 10));
    } else if (c == Board::OO) {
        painter.setPen(QPen(Qt::red, 5));
    } else if (c == Board::XX) {
        painter.setPen(QPen(Qt::blue, 5));
    }

    if (c == Board::OO) {
        painter.drawEllipse(
            4+b*kBoardSize+x*kGridSize+5,
            4+h+b*kBoardSize+y*kGridSize+5,
            kGridSize-10,
            kGridSize-10);
    } else if (c == Board::XX) {
        painter.drawLine(
            4+b*kBoardSize+x*kGridSize+5,
            4+h+b*kBoardSize+y*kGridSize+5,
            4+b*kBoardSize+x*kGridSize+kGridSize-5,
            4+h+b*kBoardSize+y*kGridSize+kGridSize-5);
        painter.drawLine(
            4+b*kBoardSize+x*kGridSize+kGridSize-5,
            4+h+b*kBoardSize+y*kGridSize+5,
            4+b*kBoardSize+x*kGridSize+5,
            4+h+b*kBoardSize+y*kGridSize+kGridSize-5);
    }
}

void MainWindow::paintEvent(QPaintEvent*) {
    cerr << "PaintEvent" << endl;
    if (board_.getState() == Board::kNotStart)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
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
                if (board_.board_[b][x][y] != Board::NN) {
                    drawGrid(painter, b, x, y, board_.board_[b][x][y]);
                }
            }
        }
    }
}

void MainWindow::updateState(int stat) {
    update();
    const QString title = "TicTacToe X 4";
    if (stat == Board::kFirstWin) {
        if (board_.isFirstPlay()) {
            QMessageBox::information(this, title, "You Win");
        } else {
            QMessageBox::information(this, title, "You Lose");
        }
    } else if (stat == Board::kSecondWin) {
        if (board_.isFirstPlay()) {
            QMessageBox::information(this, title, "You Lose");
        } else {
            QMessageBox::information(this, title, "You Win");
        }
    } else if (stat == Board::kDraw) {
        QMessageBox::information(this, title, "Draw Game");
    }
}
