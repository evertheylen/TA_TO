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

private:
    Ui::secDialog *ui;
  // QLineEdit searchstr;
};

#endif // SECDIALOG_H
