#ifndef MINEWIDGET_H
#define MINEWIDGET_H

#include <vector>
#include <QWidget>
#include <QPaintEvent>

using namespace std;

class MineWidget : public QWidget
{
Q_OBJECT

public:
    enum State {
        kHide = 0,
        kBlank = 1,
        kMine = 2,
        kFlag = 3,
        kQuestion = 4,
        kNumber = 5,
        kEnd = kNumber+8
    };

    explicit MineWidget(QWidget *parent = nullptr);
    void start(size_t width, size_t height, size_t mines);

protected:
    void paintEvent(QPaintEvent* e) override;

signals:

public slots:

private:
    size_t width_;
    size_t height_;
    size_t mines_;
    size_t percent_;
    vector<vector<int>> state_;
    vector<vector<bool>> board_;
};

#endif // MINEWIDGET_H
