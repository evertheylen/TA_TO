#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "../etc/file.h"
#include "../etc/filemanager.h"
#include "../engine_s/search.h"


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
	//std::vector <File*> files;
	Filemanager manager;

    ~MainWindow();

private slots:
    //void on_results_clicked();

    void on_addtestbutton_clicked();

    void on_pushButton_clicked();

    void on_quitprogram_clicked();

    void on_runtests_clicked();

    void on_tableWidget_cellClicked(int row, int column);

	void on_offload_button_clicked();

	void on_add_dir_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
