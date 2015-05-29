#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "../etc/file.h"
#include "../engine_s/search.h"

struct testsearch {
    int total_error;
    int fake_error;
    int skip_error;
    int ignore_error;
    int repetition_error;
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
	int querycount;

    std::vector <Query> queries;
    std::vector <File*> files;

    ~MainWindow();

private slots:
    //void on_results_clicked();

    void on_addtestbutton_clicked();

    void on_pushButton_clicked();

    void on_quitprogram_clicked();

    void on_runtests_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;

	void rebuild_table();
};

#endif // MAINWINDOW_H
