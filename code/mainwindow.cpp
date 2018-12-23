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

   ui->meni->addMenu(ui->file);
   ui->file->addAction(ui->quit);
   connect(ui->quit, SIGNAL(triggered()), this, SLOT(close()));
}

Main::~Main()
{
    delete ui;
}


