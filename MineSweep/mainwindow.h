#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void restart(void);
    void startSimple(void);
    void startMedium(void);
    void startHard(void);
    void startCustom(void);

private:
    int width_;
    int height_;
    Ui::MainWindow *ui_;
};

#endif // MAINWINDOW_H
