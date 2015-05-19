#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <list>
#include "../etc/file.h"

struct testsearch {
    int error;
    std::string searchstr;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    std::list <testsearch> tests;
    Suffix3* suffixtree;

    ~MainWindow();

private slots:
    void on_results_clicked();

    void on_addtestbutton_clicked();

    void on_pushButton_clicked();

    void on_quitprogram_clicked();

    void on_runtests_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
