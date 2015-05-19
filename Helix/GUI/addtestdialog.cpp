#include "addtestdialog.h"
#include "ui_addtestdialog.h"

addtestdialog::addtestdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addtestdialog)
{
    ui->setupUi(this);
}

addtestdialog::~addtestdialog()
{
    delete ui;
}


void addtestdialog::on_gobacktomain_clicked()
{
   this->close();
}

void addtestdialog::on_pushButton_clicked()
{
    //This function will add a test file thats specified in the plain text edit frame
}
