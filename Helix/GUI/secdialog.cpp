#include "secdialog.h"
#include "ui_secdialog.h"
#include "QString"
#include "mainwindow.h"
#include <iostream>

secDialog::secDialog(QWidget *parent) :
    QDialog(parent),
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
    std::cout << "test added successfully search for " << search << " with " << error << " amount of errors.\n";
   // this->parent()->tests.push_back(t);
}
