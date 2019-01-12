#include "mainwindow.h"
#include "gameserver.h"
#include <QApplication>
#include <QDebug>

Gameserver *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new Gameserver();
    w->show();

    return a.exec();
}
