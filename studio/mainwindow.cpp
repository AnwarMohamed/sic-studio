#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    resizeEvent(NULL);
}

void MainWindow::resizeEvent(QResizeEvent*) {
    ui->asmFrame->resize(geometry().width() - 1,
                geometry().height() - ui->mainToolBar->geometry().height() - 42);
}

MainWindow::~MainWindow() {
    delete ui;
}

