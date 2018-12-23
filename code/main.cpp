#include "mainwindow.h"
#include "second.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Second w;
    w.show();

    return a.exec();
}
