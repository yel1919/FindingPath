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

void MainWindow::on_pbGenerate_clicked() {
    ui->map->Create(ui->sbWidth->text().toInt(), ui->sbHeight->text().toInt());
}
