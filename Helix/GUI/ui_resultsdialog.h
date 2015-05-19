/********************************************************************************
** Form generated from reading UI file 'resultsdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTSDIALOG_H
#define UI_RESULTSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_resultsdialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;
    QPushButton *gobackttomainfromresults;

    void setupUi(QDialog *resultsdialog)
    {
        if (resultsdialog->objectName().isEmpty())
            resultsdialog->setObjectName(QString::fromUtf8("resultsdialog"));
        resultsdialog->resize(634, 366);
        label = new QLabel(resultsdialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 10, 59, 16));
        label_2 = new QLabel(resultsdialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(550, 0, 81, 51));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/:/logos/HELIX2_negatief.png")));
        label_2->setScaledContents(true);
        widget = new QWidget(resultsdialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 50, 631, 311));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(widget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        gridLayout->addWidget(textBrowser, 0, 0, 1, 1);

        gobackttomainfromresults = new QPushButton(widget);
        gobackttomainfromresults->setObjectName(QString::fromUtf8("gobackttomainfromresults"));

        gridLayout->addWidget(gobackttomainfromresults, 1, 0, 1, 1);


        retranslateUi(resultsdialog);

        QMetaObject::connectSlotsByName(resultsdialog);
    } // setupUi

    void retranslateUi(QDialog *resultsdialog)
    {
        resultsdialog->setWindowTitle(QApplication::translate("resultsdialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QString());
        gobackttomainfromresults->setText(QApplication::translate("resultsdialog", "Go back", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class resultsdialog: public Ui_resultsdialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTSDIALOG_H
