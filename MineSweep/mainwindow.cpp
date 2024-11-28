#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QResizeEvent>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow) {
    ui_->setupUi(this);

    setWindowTitle(tr("Mines"));
    connect(ui_->changeLevel, &QPushButton::clicked, this, &MainWindow::restart);
    connect(ui_->cancel, &QPushButton::clicked, this, &MainWindow::restart);

    connect(ui_->simple, &QPushButton::clicked, this, &MainWindow::startSimple);
    connect(ui_->simple, &QPushButton::clicked, this, &MainWindow::startMedium);
    connect(ui_->simple, &QPushButton::clicked, this, &MainWindow::startHard);
    connect(ui_->custom, &QPushButton::clicked, this, &MainWindow::startCustom);
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    cerr << "Resize: (" << e->size().width()
         << "," << e->size().height()
         << ")" << endl;

    auto size = e->size();

    QMainWindow::resizeEvent(e);
}

void MainWindow::restart(void) {
    ui_->stackedWidget->setCurrentIndex(0);
}

void MainWindow::startSimple(void) {
    ui_->stackedWidget->setCurrentIndex(1);
}

void MainWindow::startMedium(void) {
    ui_->stackedWidget->setCurrentIndex(1);
}

void MainWindow::startHard(void) {
    ui_->stackedWidget->setCurrentIndex(1);
}

void MainWindow::startCustom(void) {
    ui_->stackedWidget->setCurrentIndex(2);
}
