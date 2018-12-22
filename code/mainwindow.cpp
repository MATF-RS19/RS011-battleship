#include "mainwindow.h"
#include "ui_main.h"

#include <QPainter>
#include <QPaintEvent>

Main::Main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main)
{
    ui->setupUi(this);
}

Main::~Main()
{
    delete ui;
}

void Main::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.setPen(Qt::black);

    QRect r1 = e->rect();
    r1.setRect(80, 100, 200, 200);
    for (int i = 1; i < 10; i++) {
        painter.drawLine(80+i*20, 100, 80+i*20, 300);
        painter.drawLine(80, 100+i*20, 280, 100+i*20);
    }
    painter.drawRect(r1);

    QRect r2 = e->rect();
    r2.setRect(360, 100, 200, 200);
    for (int i = 1; i < 10; i++) {
        painter.drawLine(360+i*20, 100, 360+i*20, 300);
        painter.drawLine(360, 100+i*20, 560, 100+i*20);
    }
    painter.drawRect(r2);
}
