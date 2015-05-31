#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QDialog>
#include "../etc/file.h"
#include <string>

namespace Ui {
class Fileview;
}

class Fileview : public QDialog
{
    Q_OBJECT

public:
    explicit Fileview(QWidget *parent = 0);
    void set_file(std::string filename);
    File* f;
    bool saved;
    ~Fileview();

private slots:
    void on_cancel_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Fileview *ui;
    std::string path;
};

#endif // FILEVIEW_H
