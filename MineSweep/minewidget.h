#ifndef MINEWIDGET_H
#define MINEWIDGET_H

#include <vector>

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

using namespace std;

class MineWidget : public QWidget
{
Q_OBJECT

public:
    enum GameState {
        kNotStarted = 0,
        kStarted = 1,
        kEnded = 2
    };

    enum GridState {
        kNormal = 0,
        kNumber = 1,
        kNull = 9,
        kMine = 10,
        kExploded = 11,
        kFlag = 12,
        kErrFlag = 13,
        kQuestion = 14,
        kPressed = 32
    };

    explicit MineWidget(QWidget *parent = nullptr);
    void start(int width, int height, int mines);
    void startBoard(int x, int y);
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

private:
    void drawGrid(QPainter& painter, int x, int y);
    void drawNormal(QPainter& painter, int x, int y);
    void drawNull(QPainter& painter, int x, int y);
    void drawPressed(QPainter& painter, int x, int y);
    void drawFlag(QPainter& painter, int x, int y);
    void drawFlag(QPainter& painter, const QRect& rect);
    void drawClock(QPainter& painter, int x, int y);
    void drawQuestion(QPainter& painter, int x, int y);
    void drawMine(QPainter& painter, int x, int y);
    void drawExploded(QPainter& painter, int x, int y);
    void drawNumber(QPainter& painter, int x, int y, int n);

signals:

public slots:

private:
    int gsize_; // grid size
    int width_;
    int height_;
    int mines_;
    int percent_;
    int mouseX_; // last mouse x position in grid coordinate
    int mouseY_; // last mouse y position in grid coordinate
    int gameState_; // game state
    vector<vector<bool>> board_;
    vector<vector<int>> state_;
};

#endif // MINEWIDGET_H
