#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secdialog.h"
#include "QtGui"


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

void MainWindow::on_pushButton_clicked()
{
    secDialog secdialog;
    secdialog.setWindowTitle("The real Helix");
    secdialog.setModal(true);
    secdialog.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}
