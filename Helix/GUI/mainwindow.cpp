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
#include "fileview.h"

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
    delete ui;
}

void MainWindow::on_pushButton_clicked()        // Input file knop :p
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    std::string fileName_str = fileName.toUtf8().constData();
	if (fileName_str != "") {
		File* f = manager.add_file(fileName_str);

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


void MainWindow::on_add_dir_clicked()
{
	QString dirName = QFileDialog::getExistingDirectory();
	if (dirName != "") {
		QStringList nameFilter("*.suffix");
		QDir directory(dirName);
		QStringList suffixFiles = directory.entryList(nameFilter);
		int row = ui->tableWidget->rowCount();
		for (auto fileName_str: suffixFiles) {
			File* f = manager.add_file((dirName+"/"+fileName_str).toStdString());

			ui->tableWidget->insertRow(ui->tableWidget->rowCount());
			QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(f->get_name()));
			QString comments = QString::fromStdString(f->comments);
			if (comments == "") {
				comments = "no comments available for this file\n";
			}
			item->setToolTip(comments);
			ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, item);
			ui->tableWidget->resizeColumnsToContents();
			row++;
		}
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
       // std::cout << " Pre vector ======  Testcount = " << querycount << " queries.size() = " << queries.size() << std::endl;
        Query query = queries.at(querycount-2);
       // std::cout << " Post vector\n";
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
	if (manager.next_ID == 0){
		QMessageBox::critical(this, tr("Error"),tr("Please specify a file."));
        return;
    }
    int size = queries.size();
    if (size != 0){
        double progress = 0.0;
		double progress_advance = 100.0/size;
		// critical step here (when offloading); for every file...
		for (int j = 0; j < manager.next_ID; j++) {
			bool hasQuery = false;
			for (Query& q: queries) {
				if (q.results_per_file.find(j) == q.results_per_file.end()) {
					hasQuery = true;
					break;
				}
			}

			if (!hasQuery) continue;

			File* file= manager.get_file(j); // might take a loooong time
            for (int i = 0; i < size; i++) {
				queries.at(i).search(file, j);
				Result& r = queries.at(i).results_per_file[j];
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
    /*(if (column == 0){//First column will show the fasta comments
		if (!manager.offload) {
		// TODO Stijn :)
//		 Fileview fv;
//		 fv.f = files.at(row);
//		 fv.set_file(fv.f->path);
//		 fv.exec();
//		 if (fv.saved) {
//			 //fv.f = fv.f;
//			 for (int i=0; i<queries.size(); i++) {
//				 queries[i].results_per_file.erase(row);
//				 ui->tableWidget->item(row, 1+i)->setText("");
//				 ui->tableWidget->item(row, 0)->setToolTip(QString::fromStdString(fv.f->comments));
//			 }
//		 }
        } else {
			QMessageBox::information(this, tr("Editing disabled"), tr("Editing disabled when offloading files to disk. Will give unexpected results."));
		}
    }*/ if (column != 0) {
		if (queries.at(column-1).results_per_file.find(row) != queries.at(column-1).results_per_file.end()) {
            //std::cout << "clicked\n";
			//This will open a new window with detailed results
            if (queries.at(column-1).results_per_file[row].matches.size() > 10000) {
                QMessageBox::critical(this, tr("Error"), tr("Qt can't handle so many matches! Sorry :("));
				return;
            }
			ResultView* resultv = new ResultView();
			queries.at(column-1).results_per_file[row].file = manager.get_file(row);
			resultv->setResult(&(queries.at(column-1).results_per_file[row]));
			resultv->show();
		} else {
			QMessageBox::critical(this, tr("Error"), tr("There are no results for this query and file yet."));
		}
	}
}

void MainWindow::on_offload_button_clicked()
{
	if (ui->offload_button->isChecked()) {
		manager.enable_offloading();
		ui->offload_button->setText("offload to disk = on");
	} else {
		manager.disable_offloading();
		ui->offload_button->setText("offload to disk = off");
	}
}
