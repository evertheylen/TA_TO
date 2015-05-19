/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *quitprogram;
    QLabel *label;
    QPushButton *addtestbutton;
    QPushButton *runtests;
    QPushButton *results;
    QLabel *label_2;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QMenu *menuHELIX;
    QMenu *menuDNA_matching;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(665, 337);
        MainWindow->setMaximumSize(QSize(16000, 16000));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(0, 0, 181, 31));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        quitprogram = new QPushButton(centralWidget);
        quitprogram->setObjectName(QString::fromUtf8("quitprogram"));
        quitprogram->setGeometry(QRect(480, 270, 186, 31));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(quitprogram->sizePolicy().hasHeightForWidth());
        quitprogram->setSizePolicy(sizePolicy);
        quitprogram->setAutoFillBackground(true);
        quitprogram->setStyleSheet(QString::fromUtf8(""));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(580, 0, 81, 51));
        label->setPixmap(QPixmap(QString::fromUtf8(":/:/logos/HELIX2_negatief.png")));
        label->setScaledContents(true);
        addtestbutton = new QPushButton(centralWidget);
        addtestbutton->setObjectName(QString::fromUtf8("addtestbutton"));
        addtestbutton->setGeometry(QRect(0, 20, 181, 31));
        addtestbutton->setStyleSheet(QString::fromUtf8(""));
        runtests = new QPushButton(centralWidget);
        runtests->setObjectName(QString::fromUtf8("runtests"));
        runtests->setGeometry(QRect(0, 270, 181, 31));
        results = new QPushButton(centralWidget);
        results->setObjectName(QString::fromUtf8("results"));
        results->setGeometry(QRect(0, 140, 181, 31));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(380, 40, 111, 16));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(385, 61, 281, 211));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 665, 22));
        menuHELIX = new QMenu(menuBar);
        menuHELIX->setObjectName(QString::fromUtf8("menuHELIX"));
        menuHELIX->setStyleSheet(QString::fromUtf8("border:none;"));
        menuDNA_matching = new QMenu(menuBar);
        menuDNA_matching->setObjectName(QString::fromUtf8("menuDNA_matching"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuHELIX->menuAction());
        menuBar->addAction(menuDNA_matching->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "Add FASTA file", 0, QApplication::UnicodeUTF8));
        quitprogram->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        addtestbutton->setText(QApplication::translate("MainWindow", "Add test file", 0, QApplication::UnicodeUTF8));
        runtests->setText(QApplication::translate("MainWindow", "Run tests", 0, QApplication::UnicodeUTF8));
        results->setText(QApplication::translate("MainWindow", "Watch result details", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Summary results:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
