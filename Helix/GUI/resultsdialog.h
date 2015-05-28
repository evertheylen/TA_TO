#ifndef RESULTSDIALOG_H
#define RESULTSDIALOG_H

#include <QDialog>
#include <QString>
#include <../etc/file.h>

namespace Ui {
class resultsdialog;
}

class resultsdialog : public QDialog
{
    Q_OBJECT

public:
    explicit resultsdialog(QWidget *parent = 0);
    ~resultsdialog();
    void set_texts(QString filename, QString searchstr);
    // hier moet het result komen

private slots:

    void on_gobackttomainfromresults_clicked();


private:
    Ui::resultsdialog *ui;
};

#endif // RESULTSDIALOG_H
