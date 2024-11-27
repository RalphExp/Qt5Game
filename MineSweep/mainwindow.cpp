#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QResizeEvent>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->simple, &QPushButton::clicked, this, &MainWindow::startSimple);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    QMainWindow::resizeEvent(e);
}

void MainWindow::startSimple(void) {
    auto stackedWidget = ui->stackedWidget;
    stackedWidget->setCurrentIndex(1);
}
