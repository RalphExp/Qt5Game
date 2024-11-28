#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QResizeEvent>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), width_(0), height_(0), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);

    setWindowTitle(tr("Mines"));
    connect(ui_->changeLevel, &QPushButton::clicked, this, &MainWindow::restart);
    connect(ui_->cancel, &QPushButton::clicked, this, &MainWindow::restart);

    connect(ui_->simple, &QPushButton::clicked, this, &MainWindow::startSimple);
    connect(ui_->medium, &QPushButton::clicked, this, &MainWindow::startMedium);
    connect(ui_->hard, &QPushButton::clicked, this, &MainWindow::startHard);
    connect(ui_->custom, &QPushButton::clicked, this, &MainWindow::startCustom);
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    QMainWindow::resizeEvent(e);
}

void MainWindow::restart(void) {
    ui_->stackedWidget->setCurrentIndex(0);
}

void MainWindow::startSimple(void) {
    ui_->stackedWidget->setCurrentIndex(1);
    ui_->mineWidget->start(8, 8, 10);
}

void MainWindow::startMedium(void) {
    ui_->stackedWidget->setCurrentIndex(1);
    ui_->mineWidget->start(16, 16, 30);
}

void MainWindow::startHard(void) {
    ui_->stackedWidget->setCurrentIndex(1);
    ui_->mineWidget->start(16, 30, 99);
}

void MainWindow::startCustom(void) {
    ui_->stackedWidget->setCurrentIndex(2);
}
