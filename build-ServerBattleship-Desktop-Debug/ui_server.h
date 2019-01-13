/********************************************************************************
** Form generated from reading UI file 'server.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_H
#define UI_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Server
{
public:
    QLabel *label;
    QPushButton *button;
    QLineEdit *lineEdit;
    QLabel *status;

    void setupUi(QDialog *Server)
    {
        if (Server->objectName().isEmpty())
            Server->setObjectName(QStringLiteral("Server"));
        Server->resize(400, 350);
        label = new QLabel(Server);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 50, 300, 25));
        button = new QPushButton(Server);
        button->setObjectName(QStringLiteral("button"));
        button->setGeometry(QRect(150, 270, 100, 25));
        lineEdit = new QLineEdit(Server);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(100, 160, 200, 25));
        status = new QLabel(Server);
        status->setObjectName(QStringLiteral("status"));
        status->setGeometry(QRect(50, 215, 300, 25));
        status->setAlignment(Qt::AlignCenter);

        retranslateUi(Server);

        QMetaObject::connectSlotsByName(Server);
    } // setupUi

    void retranslateUi(QDialog *Server)
    {
        Server->setWindowTitle(QApplication::translate("Server", "Dialog", Q_NULLPTR));
        label->setText(QString());
        button->setText(QApplication::translate("Server", "Quit", Q_NULLPTR));
        status->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Server: public Ui_Server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_H
