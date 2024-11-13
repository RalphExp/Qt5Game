#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPen>

#include "board.h"

namespace Ui {
class MainWindow;
}

class QPen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum {
      kGridSize = 32,
      kBoardSize = kGridSize*4,
      kPanelSize = kBoardSize*4,
      kPenSize = 2
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void startNewGame(void);
    void quitGame(void);
    void drawGrid(QPainter& painter, int board, int x, int y, int color);
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::MainWindow *ui_;
    int menu_height_;
    QPen pen_;
    Board board_;
};

#endif // MAINWINDOW_H
