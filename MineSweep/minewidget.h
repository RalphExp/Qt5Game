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
    enum State {
        kNormal = 0,
        kPressed = 1,
        kMine = 2,
        kRedmine = 3,
        kFlag = 4,
        kQuestion = 5,
        kNumber = 6,
        kEnd = kNumber+7
    };

    explicit MineWidget(QWidget *parent = nullptr);
    void start(int width, int height, int mines);
    void paintEvent(QPaintEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void drawGrid(QPainter& painter, int x, int y);

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
    vector<vector<int>> state_;
    vector<vector<bool>> board_;
};

#endif // MINEWIDGET_H
