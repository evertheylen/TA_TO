#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secdialog.h"
#include "addtestdialog.h"
#include "resultsdialog.h"
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
    secdialog.setWindowTitle("Add FASTA file");
    secdialog.setModal(true);
    secdialog.exec();
}

void MainWindow::on_quitprogram_clicked()
{
    this->close();
}

void MainWindow::on_addtestbutton_clicked()
{
    addtestdialog addtest;
    addtest.setWindowTitle("Add test file");
    addtest.setModal(true);
    addtest.exec();
}

void MainWindow::on_results_clicked()
{
    //This button will show us the detailed results from the test
    resultsdialog results;
    results.setWindowTitle("Detailed results");
    results.setModal(true);
    results.exec();
}

void MainWindow::on_runtests_clicked()
{
    //This button will run the tests
}
