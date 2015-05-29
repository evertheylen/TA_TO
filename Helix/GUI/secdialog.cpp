#include "secdialog.h"
#include "ui_secdialog.h"
#include "QString"
#include "mainwindow.h"
#include <iostream>
#include "QMessageBox"

secDialog::secDialog(QWidget *parent) :
    QDialog(parent), check(true),
    ui(new Ui::secDialog)
{
    ui->setupUi(this);
}

secDialog::~secDialog()
{
    delete ui;
}

void secDialog::on_pushButton_clicked()
{
    this->close();
}

void secDialog::on_ADD_clicked()
{
    QString content = ui->searchstr->text();
    std::string search = content.toUtf8().constData();
    //std::cout << search << std::endl;

    int total_error  = ui->erroramount->value();
    int fake_error  = ui->erroramount_3->value();
    int skip_error  = ui->erroramount_4->value();
    int repetition_error  = ui->erroramount_5->value();
    int ignore_error  = ui->erroramount_6->value();
    //std::cout << error << std::endl;
    std::cout << ((MainWindow*)parentWidget())->queries.size() << std::endl;
	((MainWindow*)parentWidget())->queries.emplace_back(
				Query(search, fake_error, skip_error, repetition_error, ignore_error, total_error));
    std::cout << ((MainWindow*)parentWidget())->queries.size() << std::endl;
	//QMessageBox::information(this, tr("New test"),tr("The test was added successfully"));

    //std::cout << "test added successfully search for " << search << " with " << error << " amount of errors.\n";
   // this->parent()->tests.push_back(t);
}

void secDialog::on_erroramount_valueChanged(int arg1)
{
    if (!ui->checkBox->isChecked()) {
        check = false;
        ui->erroramount_3->setValue(ui->erroramount->value());
    }
    if (!ui->checkBox_2->isChecked()) {
        check = false;
        ui->erroramount_4->setValue(ui->erroramount->value());
    }
    if (!ui->checkBox_3->isChecked()) {
        check = false;
        ui->erroramount_5->setValue(ui->erroramount->value());
    }
    if (!ui->checkBox_4->isChecked()) {
        check = false;
        ui->erroramount_6->setValue(ui->erroramount->value());
    }

}

void secDialog::on_checkBox_clicked()
{
    if (!ui->checkBox->isChecked()) {
        ui->erroramount_3->setValue(ui->erroramount->value());
    }
}

void secDialog::on_checkBox_2_clicked()
{
    if (!ui->checkBox_2->isChecked()) {
        ui->erroramount_4->setValue(ui->erroramount->value());
    }
}

void secDialog::on_checkBox_3_clicked()
{
    if (!ui->checkBox_3->isChecked()) {
        ui->erroramount_5->setValue(ui->erroramount->value());
    }
}

void secDialog::on_checkBox_4_clicked()
{
    if (!ui->checkBox_4->isChecked()) {
        ui->erroramount_6->setValue(ui->erroramount->value());
    }
}

void secDialog::on_erroramount_3_valueChanged(int arg1)
{
    if (!ui->checkBox->isChecked() && check) {
        ui->checkBox->setChecked(true);
    } check = true;
    if (ui->erroramount_3->value() > ui->erroramount->value()) {
        ui->erroramount_3->setValue(ui->erroramount->value());
    }
}

void secDialog::on_erroramount_4_valueChanged(int arg1)
{
    if (!ui->checkBox_2->isChecked() && check) {
        ui->checkBox_2->setChecked(true);
    } check = true;
    if (ui->erroramount_4->value() > ui->erroramount->value()) {
        ui->erroramount_4->setValue(ui->erroramount->value());
    }
}

void secDialog::on_erroramount_5_valueChanged(int arg1)
{
    if (!ui->checkBox_3->isChecked() && check) {
        ui->checkBox_3->setChecked(true);
    } check = true;
    if (ui->erroramount_5->value() > ui->erroramount->value()) {
        ui->erroramount_5->setValue(ui->erroramount->value());
    }
}

void secDialog::on_erroramount_6_valueChanged(int arg1)
{
    if (!ui->checkBox_4->isChecked() && check) {
        ui->checkBox_4->setChecked(true);
    } check = true;
    if (ui->erroramount_6->value() > ui->erroramount->value()) {
        ui->erroramount_6->setValue(ui->erroramount->value());
    }
}
