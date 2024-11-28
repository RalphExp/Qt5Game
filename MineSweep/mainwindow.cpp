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

    width_ = width();
    height_ = height();
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::resizeEvent(QResizeEvent *e) {
    QMainWindow::resizeEvent(e);
}

void MainWindow::restart(void) {
    ui_->stackedWidget->setCurrentIndex(0);
    setFixedSize(width_, height_);
}

void MainWindow::startSimple(void) {
    ui_->stackedWidget->setCurrentIndex(1);
    ui_->mineWidget->setFixedSize(8*40, 8*40);
    ui_->frame->setFixedHeight(8*40);
    setFixedSize(8*40+240, 8*40+80);
}

void MainWindow::startMedium(void) {
    ui_->stackedWidget->setCurrentIndex(1);
    ui_->mineWidget->setFixedSize(16*32, 16*32);
    ui_->frame->setFixedHeight(16*32);
    setFixedSize(16*32+240, 16*32+80);
}

void MainWindow::startHard(void) {
    ui_->stackedWidget->setCurrentIndex(1);
    ui_->mineWidget->setFixedSize(30*32, 16*32);
    ui_->frame->setFixedHeight(16*32);
    setFixedSize(30*32+240, 16*32+80);
}

void MainWindow::startCustom(void) {
    ui_->stackedWidget->setCurrentIndex(2);
}
