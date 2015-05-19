/********************************************************************************
** Form generated from reading UI file 'addtestdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDTESTDIALOG_H
#define UI_ADDTESTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_addtestdialog
{
public:
    QLabel *label;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QPlainTextEdit *plainTextEdit;
    QPushButton *gobacktomain;
    QPushButton *pushButton;

    void setupUi(QDialog *addtestdialog)
    {
        if (addtestdialog->objectName().isEmpty())
            addtestdialog->setObjectName(QString::fromUtf8("addtestdialog"));
        addtestdialog->resize(672, 374);
        label = new QLabel(addtestdialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(580, 0, 81, 51));
        label->setPixmap(QPixmap(QString::fromUtf8(":/:/logos/HELIX2_negatief.png")));
        label->setScaledContents(true);
        widget = new QWidget(addtestdialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(7, 24, 661, 351));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setPointSize(18);
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 0, 0, 1, 2);

        plainTextEdit = new QPlainTextEdit(widget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 1, 0, 1, 2);

        gobacktomain = new QPushButton(widget);
        gobacktomain->setObjectName(QString::fromUtf8("gobacktomain"));

        gridLayout->addWidget(gobacktomain, 2, 0, 1, 1);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 2, 1, 1, 1);


        retranslateUi(addtestdialog);

        QMetaObject::connectSlotsByName(addtestdialog);
    } // setupUi

    void retranslateUi(QDialog *addtestdialog)
    {
        addtestdialog->setWindowTitle(QApplication::translate("addtestdialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QApplication::translate("addtestdialog", "Specify the test file here:", 0, QApplication::UnicodeUTF8));
        gobacktomain->setText(QApplication::translate("addtestdialog", "Go back", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("addtestdialog", "Add test file", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class addtestdialog: public Ui_addtestdialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDTESTDIALOG_H
