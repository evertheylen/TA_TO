/********************************************************************************
** Form generated from reading UI file 'secdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECDIALOG_H
#define UI_SECDIALOG_H

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

class Ui_secDialog
{
public:
    QLabel *label;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton;
    QPushButton *ADD;

    void setupUi(QDialog *secDialog)
    {
        if (secDialog->objectName().isEmpty())
            secDialog->setObjectName(QString::fromUtf8("secDialog"));
        secDialog->resize(649, 361);
        secDialog->setStyleSheet(QString::fromUtf8(""));
        label = new QLabel(secDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(550, 0, 91, 41));
        label->setPixmap(QPixmap(QString::fromUtf8(":/:/logos/HELIX2_negatief.png")));
        label->setScaledContents(true);
        widget = new QWidget(secDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 21, 651, 341));
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
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-color: white;\n"
""));

        gridLayout->addWidget(plainTextEdit, 1, 0, 1, 2);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Helvetica"));
        font1.setPointSize(13);
        pushButton->setFont(font1);
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setAutoDefault(true);
        pushButton->setDefault(false);
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 2, 0, 1, 1);

        ADD = new QPushButton(widget);
        ADD->setObjectName(QString::fromUtf8("ADD"));
        ADD->setFont(font1);
        ADD->setStyleSheet(QString::fromUtf8(""));
        ADD->setAutoDefault(true);
        ADD->setDefault(false);
        ADD->setFlat(false);

        gridLayout->addWidget(ADD, 2, 1, 1, 1);

        pushButton->raise();
        label->raise();
        label_2->raise();
        plainTextEdit->raise();
        label_2->raise();
        ADD->raise();

        retranslateUi(secDialog);

        QMetaObject::connectSlotsByName(secDialog);
    } // setupUi

    void retranslateUi(QDialog *secDialog)
    {
        secDialog->setWindowTitle(QApplication::translate("secDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QApplication::translate("secDialog", "Specify the FASTA file here:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("secDialog", "Go back", 0, QApplication::UnicodeUTF8));
        ADD->setText(QApplication::translate("secDialog", "Add file", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class secDialog: public Ui_secDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECDIALOG_H
