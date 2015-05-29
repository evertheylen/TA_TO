#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secdialog.h"
#include "addtestdialog.h"
#include "resultview.h"
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
#include "../etc/file.h"

#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),querycount(1),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);  // ?
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < files.size(); i++) {
        delete files.at(i);
    }
    delete ui;
}

void MainWindow::on_pushButton_clicked()        // Input file knop :p
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    std::string fileName_str = fileName.toUtf8().constData();
	if (fileName_str != "") {
        File* f = new File(fileName_str, files.size());
        files.emplace_back(f);  // emplace_back zorgt ervoor dat het direct op de juiste plaats wordt geinit

        QMessageBox::information(this, tr("Suffixtree"), tr("The file was loaded and the suffixtree was created"));
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(f->get_name()));
        QString comments = QString::fromStdString(f->comments);
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
}

// enkel te gebruiken voor cellen te vervangen
void MainWindow::rebuild_table() {
    for (File* f: files) {

	}
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

 //   std::cout << tests.size() << std::endl;
    for (int i=querycount-1; i<queries.size(); i++) {
        std::string newquery;
        ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
        newquery = "Query " + std::to_string(querycount);
        querycount++;
        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(newquery));
        QString tooltip = "Search for ";
        std::cout << " Pre vector ======  Testcount = " << querycount << " queries.size() = " << queries.size() << std::endl;
        Query query = queries.at(querycount-2);
        std::cout << " Post vector\n";
        tooltip += QString::fromStdString(query.input);
        tooltip += " with ";
        tooltip += QString::number(query.max_total);
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
    if (files.size() == 0){
		QMessageBox::critical(this, tr("Error"),tr("Please specify a file."));
        return;
    }
    int size = queries.size();
    if (size != 0){
        double progress = 0.0;
        double progress_advance = 100/size;
        for (int j = 0; j < files.size(); j++) {
            for (int i = 0; i < size; i++) {
				queries.at(i).search(*(files.at(j)));
				Result& r = queries.at(i).results_per_file[files.at(j)->ID];
                QTableWidgetItem* item = new QTableWidgetItem();
				item->setText(QString::fromStdString(r.summary()));
                ui->tableWidget->setItem(j, i+1, item);
                ui->tableWidget->resizeColumnsToContents();
                //std::cout << ui->tableWidget->item(j, i+1) << std::endl;
                //ui->tableWidget->item(j+1, i+1)->setText(match);
                //std::cout << "Set new item to " << match.toStdString() << std::endl;
                //std::cout << "Done\n";
                progress += progress_advance;
				ui->progressBar->setValue(round(progress));
            }
        }
    }
    else{
		QMessageBox::critical(this, tr("Error"),tr("Please specify a query."));
    }
}


void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if (column == 0){//First column will show the fasta comments
      //  QMessageBox::information(this, tr("Fasta comments"), tr("The comments of the fasta file will appear here: "));
    }
    else{
        //This will open a new window with detailed results
		ResultView resultv;
		resultv.show();
    }
}
