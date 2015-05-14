#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>


int main(int argc, char *argv[])
{
    QApplication program(argc, argv);
    MainWindow w;
    w.setStyleSheet("background-color: lightblue;");
    w.show();

    return program.exec();
}
