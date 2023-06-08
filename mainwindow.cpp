#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :
      QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QObject::tr("Finding path"));

    Load();
}

MainWindow::~MainWindow() {
    Save();

    delete ui;
}

void MainWindow::on_pbGenerate_clicked() {
    ui->map->Create(ui->sbWidth->text().toInt(), ui->sbHeight->text().toInt());
}

void MainWindow::Save() {
    settings = new QSettings(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
    settings->setValue("geometry", geometry());
    delete settings;
}
void MainWindow::Load() {
    settings = new QSettings(QDir::currentPath() + "/config.ini", QSettings::IniFormat);
    setGeometry(settings->value("geometry", QRect(0, 0, 800, 600)).toRect());
    delete settings;
}
