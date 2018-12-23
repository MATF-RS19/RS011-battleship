#include "mainwindow.h"
#include "ui_main.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPointF>
#include <QVector>
#include <QDebug>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <QGraphicsTextItem>

Main::Main(QWidget *parent) :
    QMainWindow(parent),
     ui(new Ui::Main)
{
   ui->setupUi(this);
   QPixmap pix(":/background2.jpg");
   pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
   QPalette palette;
   palette.setBrush(QPalette::Background, pix);
   this->setPalette(palette);

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

