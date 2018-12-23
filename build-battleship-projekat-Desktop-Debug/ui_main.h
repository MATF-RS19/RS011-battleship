/********************************************************************************
** Form generated from reading UI file 'main.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Main
{
public:
    QWidget *centralWidget;
    QLabel *l1;
    QLabel *l2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Main)
    {
        if (Main->objectName().isEmpty())
            Main->setObjectName(QString::fromUtf8("Main"));
        Main->resize(681, 471);
        centralWidget = new QWidget(Main);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        l1 = new QLabel(centralWidget);
        l1->setObjectName(QString::fromUtf8("l1"));
        l1->setGeometry(QRect(80, 50, 200, 20));
        l1->setAlignment(Qt::AlignCenter);
        l2 = new QLabel(centralWidget);
        l2->setObjectName(QString::fromUtf8("l2"));
        l2->setGeometry(QRect(360, 50, 200, 20));
        l2->setAlignment(Qt::AlignCenter);
        Main->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(Main);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Main->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Main);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Main->setStatusBar(statusBar);

        retranslateUi(Main);

        QMetaObject::connectSlotsByName(Main);
    } // setupUi

    void retranslateUi(QMainWindow *Main)
    {
        Main->setWindowTitle(QApplication::translate("Main", "Main", 0, QApplication::UnicodeUTF8));
        l1->setText(QApplication::translate("Main", "PLAYER 1", 0, QApplication::UnicodeUTF8));
        l2->setText(QApplication::translate("Main", "PLAYER 2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Main: public Ui_Main {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
