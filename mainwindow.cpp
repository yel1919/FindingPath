#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :
      QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QObject::tr("Finding path"));
}

MainWindow::~MainWindow() {
    delete ui;
}

