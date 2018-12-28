#include "mainwindow.h"
#include "second.h"
#include <QApplication>
#include <QDebug>

Second *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new Second();
    w->show();

    return a.exec();
}
