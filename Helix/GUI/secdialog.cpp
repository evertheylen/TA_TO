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

    int error  = ui->erroramount->value();
    //std::cout << error << std::endl;

    testsearch t;
    t.error = error;
    t.searchstr = search;
    ((MainWindow*)parentWidget())->tests.push_back(t);
    QMessageBox::information(this, tr("New test"),tr("The test was added successfully"));

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
}

void secDialog::on_erroramount_4_valueChanged(int arg1)
{
    if (!ui->checkBox_2->isChecked() && check) {
        ui->checkBox_2->setChecked(true);
    } check = true;
}

void secDialog::on_erroramount_5_valueChanged(int arg1)
{
    if (!ui->checkBox_3->isChecked() && check) {
        ui->checkBox_3->setChecked(true);
    } check = true;
}

void secDialog::on_erroramount_6_valueChanged(int arg1)
{
    if (!ui->checkBox_4->isChecked() && check) {
        ui->checkBox_4->setChecked(true);
    } check = true;
}
