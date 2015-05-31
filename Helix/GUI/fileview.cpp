#include "fileview.h"
#include "ui_fileview.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <string>
#include <fstream>
#include <QMessageBox>

Fileview::Fileview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fileview), saved(false)
{
    ui->setupUi(this);
}

Fileview::~Fileview()
{
    delete ui;
}

void Fileview::on_cancel_clicked()
{
    this->close();
}

void Fileview::on_pushButton_4_clicked()
{
    QString pad =QString::fromStdString(path);
    std::cout << "Writing to file on " << path << std::endl;
    QFile file(pad);
    file.open(QIODevice::WriteOnly);
    QTextStream output(&file);
    output << ui->textBrowser->document()->toPlainText();
    file.close();

  /*  std::ofstream file(path);
    file.open();
    file << ui->textBrowser->document()->toPlainText();
    file.close();*/
    int id = f->ID;
    f = new File(path, id);
    saved = true;
    this->close();
}

void Fileview::set_file(std::string filename) {
    QFile file (QString::fromStdString(filename));
    //std::cout << filename << std::endl;
    if(!file.open(QIODevice::ReadWrite)) {
        QMessageBox::critical(0, "Error!", file.errorString());
    }

    path = filename;
    QTextStream input(&file);

    ui->textBrowser->setText(input.readAll());
    file.close();
}
