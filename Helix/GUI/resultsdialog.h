#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include <QDialog>

namespace Ui {
class resultsdialog;
}

class resultsdialog : public QDialog
{
    Q_OBJECT

public:
    explicit resultsdialog(QWidget *parent = 0);
    ~resultsdialog();

private slots:

    void on_gobackttomainfromresults_clicked();


private:
    Ui::resultsdialog *ui;
};

#endif // RESULTSDIALOG_H
