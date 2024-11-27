#ifndef MINEWIDGET_H
#define MINEWIDGET_H

#include <QWidget>
#include <QPaintEvent>

class MineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MineWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* e) override;

signals:

public slots:
};

#endif // MINEWIDGET_H
