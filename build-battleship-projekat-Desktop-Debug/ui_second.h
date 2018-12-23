/********************************************************************************
** Form generated from reading UI file 'second.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECOND_H
#define UI_SECOND_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Second
{
public:
    QPushButton *Play;
    QPushButton *Close;

    void setupUi(QDialog *Second)
    {
        if (Second->objectName().isEmpty())
            Second->setObjectName(QString::fromUtf8("Second"));
        Second->setEnabled(true);
        Second->resize(872, 590);
        Second->setMinimumSize(QSize(475, 0));
        Play = new QPushButton(Second);
        Play->setObjectName(QString::fromUtf8("Play"));
        Play->setGeometry(QRect(380, 140, 99, 27));
        Play->setLayoutDirection(Qt::LeftToRight);
        Close = new QPushButton(Second);
        Close->setObjectName(QString::fromUtf8("Close"));
        Close->setGeometry(QRect(380, 320, 99, 27));

        retranslateUi(Second);

        QMetaObject::connectSlotsByName(Second);
    } // setupUi

    void retranslateUi(QDialog *Second)
    {
        Second->setWindowTitle(QApplication::translate("Second", "Dialog", 0, QApplication::UnicodeUTF8));
        Play->setText(QApplication::translate("Second", "Start", 0, QApplication::UnicodeUTF8));
        Close->setText(QApplication::translate("Second", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Second: public Ui_Second {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECOND_H
