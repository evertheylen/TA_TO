#ifndef ADDTESTDIALOG_H
#define ADDTESTDIALOG_H

#include <QDialog>

namespace Ui {
class addtestdialog;
}

class addtestdialog : public QDialog
{
    Q_OBJECT

public:
    explicit addtestdialog(QWidget *parent = 0);
    ~addtestdialog();

private slots:
    void on_pushButton_clicked();

    void on_gobacktomain_clicked();


private:
    Ui::addtestdialog *ui;
};

#endif // ADDTESTDIALOG_H
