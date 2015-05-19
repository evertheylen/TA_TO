#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secdialog.h"
#include "addtestdialog.h"
#include "resultsdialog.h"
#include "QtGui"
#include "QString"
#include "QDesktopServices"
#include "QFileDialog"
#include "iostream"
#include "QProcess"
#include "QLabel"

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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.txt)"));
    std::string fileName_str = fileName.toUtf8().constData();

    File f(fileName_str);
    suffixtree = f.suffixtree;
    std::cout << f.get_name() << std::endl;

    QProcess dot_to_png;

    dot_to_png.start("dot -Tpng end_0.dot -o suffix.png");
    dot_to_png.waitForFinished(-1);

    QLabel * label_img = new QLabel (this);
    label_img->setWindowFlags(Qt::Window);

    label_img->setPixmap(QPixmap("suffix.png", 0, Qt::AutoColor));
    label_img->show();
    //QString path = QDir::toNativeSeparators(QApplication::applicationDirPath());
    //QDesktopServices::openUrl(QUrl("file:///" + path));

}

void MainWindow::on_quitprogram_clicked()
{
    this->close();
}

void MainWindow::on_addtestbutton_clicked()
{
    secDialog secdialog(this);
    secdialog.setWindowTitle("Search patterns for suffix tree");
    secdialog.setModal(true);
    secdialog.exec();/*
    addtestdialog addtest;
    addtest.setWindowTitle("Add test file");
    addtest.setModal(true);
    addtest.exec();*/
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
    int size = tests.size();
    for (int i = 0; i < size; i++) {
        std::vector<int> result = suffixtree->search_string(tests.front().searchstr, tests.front().error);
        QString input = "Results for search ";
        input += QString::fromStdString(tests.front().searchstr);
        input += " with ";
        input += QString::number(tests.front().error);
        input += " errors.\nIn string ";
        input += QString::fromStdString(suffixtree->s);
        input += "\n";
        ui->textBrowser->append(input);
        for (int j = 0; j < result.size(); j++) {
            QString index = "@ index";
            index += QString::number(result.at(j));
            ui->textBrowser->append(index);
        }
        tests.pop_front();
    }
}

