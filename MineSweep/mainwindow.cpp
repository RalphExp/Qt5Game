#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QResizeEvent>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    auto stackWidget = ui->stackedWidget;
    int idx = stackWidget->currentIndex();

    if (idx == 0) { // 4 buttons
        // cerr << "MainWindow Size: " << width() << ", " << height() << endl;
        // cerr << "Button Size: " << ui->pushButton->width() << ", " << ui->pushButton->height() << endl;

//        int w = min(e->size().width()-120, e->size().height()-40)/2;
//        ui->simple->resize(w, w);
//        ui->medium->resize(w, w);
//        ui->hard->resize(w, w);
//        ui->custom->resize(w, w);

    } else if (idx == 1) { // mine panel

    }
    QMainWindow::resizeEvent(e);
}
