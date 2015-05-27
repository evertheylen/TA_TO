#ifndef SECDIALOG_H
#define SECDIALOG_H

#include <QDialog>
#include "QLineEdit"
#include "../etc/file.h"

namespace Ui {
class secDialog;
}

class secDialog : public QDialog
{
    Q_OBJECT

public:
    explicit secDialog(QWidget *parent = 0);
    ~secDialog();

private slots:
    void on_ADD_clicked();
    void on_pushButton_clicked();

    void on_erroramount_valueChanged(int arg1);

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_erroramount_3_valueChanged(int arg1);

    void on_erroramount_4_valueChanged(int arg1);

    void on_erroramount_5_valueChanged(int arg1);

    void on_erroramount_6_valueChanged(int arg1);

private:
    Ui::secDialog *ui;

    bool check;
  // QLineEdit searchstr;
};

#endif // SECDIALOG_H
