#include "mainwindow.h"
#include "gameserver.h"
#include <QApplication>
#include <QDebug>
#include <QIcon>

GameServer *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon1.png"));
    w = new GameServer();
    w->show();

    return a.exec();
}
