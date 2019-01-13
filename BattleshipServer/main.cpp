#include "server.h"
#include <QApplication>

#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon_radar.png"));
    Server s;
    s.show();
    return a.exec();
}
