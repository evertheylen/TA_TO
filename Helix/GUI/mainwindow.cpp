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
#include "QMessageBox"
#include "QTableWidgetItem"
#include "QTableWidget"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),testcount(1),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()        // Input file knop :p
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.txt)"));
    std::string fileName_str = fileName.toUtf8().constData();

    File f(fileName_str);
    suffixtrees.push_back(f.suffixtree);
    QMessageBox::information(this, tr("Suffixtree"), tr("The file was loaded and the suffixtree was created"));
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(f.get_name()));
    QString comments = QString::fromStdString(f.comments);
    if (comments == "") {
        comments = "no comments available for this file\n";
    }
    item->setToolTip(comments);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, item);
    ui->tableWidget->resizeColumnsToContents();
    //std::cout << f.get_name() << std::endl;

    //QProcess dot_to_png;

    //dot_to_png.start("dot -Tpng end_0.dot -o suffix.png");
    //dot_to_png.waitForFinished(-1);

    /*QLabel * label_img = new QLabel (this);
    label_img->setWindowFlags(Qt::Window);

    label_img->setPixmap(QPixmap("suffix.png", 0, Qt::AutoColor));
    label_img->show();*/
    //QString path = QDir::toNativeSeparators(QApplication::applicationDirPath());
    //QDesktopServices::openUrl(QUrl("file:///" + path));

}

void MainWindow::on_quitprogram_clicked()
{
    this->close();
}

void MainWindow::on_addtestbutton_clicked()     // Input new query knop
{
    secDialog secdialog(this);
    secdialog.setWindowTitle("Search patterns for suffix tree");
    secdialog.setModal(true);
    secdialog.exec();

    std::cout << tests.size() << std::endl;
    for (int i=0;i<tests.size();i++){
        std::string newtest;
        ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
        newtest = "Test " + std::to_string(testcount);
        testcount++;
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(newtest));
        QString tooltip = "Search for ";
        testsearch test = tests.at(testcount-2);
        tooltip += QString::fromStdString(test.searchstr);
        tooltip += " with ";
        tooltip += QString::number(test.total_error);
        tooltip += " as total error amount\n";
        item->setToolTip(tooltip);
        ui->tableWidget->setHorizontalHeaderItem(ui->tableWidget->columnCount()-1, item);
        ui->tableWidget->resizeColumnsToContents();
    }
    /*
    addtestdialog addtest;
    addtest.setWindowTitle("Add test file");
    addtest.setModal(true);
    addtest.exec();*/
}

/*void MainWindow::on_results_clicked()
{
    //This button will show us the detailed results from the test
    resultsdialog results;
    results.setWindowTitle("Detailed results");
    results.setModal(true);
    results.exec();
}*/

void MainWindow::on_runtests_clicked()
{
    int size = tests.size();
    double progress = 0.0;
    double progress_advance = 100/(size-1);
    for (int j = 0; j < suffixtrees.size(); j++) {
        for (int i = 0; i < size; i++) {
            std::vector<int> result = {};//suffixtrees.at(j)->search_string(tests.at(i).searchstr, tests.at(i).total_error);
            QString input = "Results for search ";
            input += QString::fromStdString(tests.at(i).searchstr);
            input += " with ";
            input += QString::number(tests.at(i).total_error);
            input += " errors.\nIn string ";
            input += QString::fromStdString(suffixtrees.at(j)->s);
            input += "\n";
            ui->textBrowser->append(input);
            QString match = QString::number(result.size());
            match += " matches\n";
            std::cout << "Setting text for item at " << j+1 << ", " << i+1 << std::endl;
            std::cout << "Item found: " <<  ui->tableWidget->item(j+1, i+1) << std::endl;
            QTableWidgetItem* item = new QTableWidgetItem(match);
            ui->tableWidget->setItem(j+1, i+1, item);
            //ui->tableWidget->item(j+1, i+1)->setText(match);
            std::cout << "Set new item to " << match.toStdString() << std::endl;
            std::cout << "Done\n";
            for (int k = 0; k < result.size(); k++) {
                QString index = "@ index";
                index += QString::number(result.at(k));
                ui->textBrowser->append(index);
            }
            progress += progress_advance;
            ui->progressBar->setValue(progress);
        }
    }
}


void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if (column == 0){//First column will show the fasta comments
        QMessageBox::information(this, tr("Fasta comments"), tr("The comments of the fasta file will appear here: "));
    }
    else{
        //This will open a new window with detailed results
        resultsdialog results;
        results.setWindowTitle("Detailed results");
        results.setModal(true);
        results.exec();
    }
}
