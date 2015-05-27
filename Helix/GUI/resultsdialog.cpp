#include "resultsdialog.h"
#include "ui_resultsdialog.h"

resultsdialog::resultsdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resultsdialog)
{
    ui->setupUi(this);
}

resultsdialog::~resultsdialog()
{
    delete ui;
}

void resultsdialog::on_gobackttomainfromresults_clicked()
{
    this->close();
}

