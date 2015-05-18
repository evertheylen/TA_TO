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
