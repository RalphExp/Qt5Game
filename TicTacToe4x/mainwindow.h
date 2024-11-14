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
    static constexpr int kGridSize = 32;
    static constexpr int kBoardSize = kGridSize*4;
    static constexpr int kPanelSize = kBoardSize*4;
    static constexpr int kPenSize = 2;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void startNewGame(void);
    void quitGame(void);
    void drawGrid(QPainter& painter, int board, int x, int y, int color);
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;

    void handleMousePress(int xpos, int ypos);
    void showBoardStat(void);

private slots:
    void updateState(int stat);

private:
    Ui::MainWindow *ui_;
    int menu_height_;
    QPen pen_;
    Board board_;
};

#endif // MAINWINDOW_H
