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

   scene = new QGraphicsScene(this);
   ui->graphicsView->setScene(scene);
   QPixmap source(":/metallic_background.jpg");
   int h = source.height();
   int w = source.width();
   source = source.scaled(w/5, h/5);
   ui->graphicsView->setBackgroundBrush(source);
   brod1 = new Ship(QString("Submarine"), 2);
   brod1->setFlag(QGraphicsItem::ItemIsSelectable, true);
   brod1->setFlag(QGraphicsItem::ItemIsMovable, true);
   scene->addItem(brod1);

   brod2 = new Ship(QString("Submarine"), 3);
   brod2->setFlag(QGraphicsItem::ItemIsSelectable, true);
   brod2->setFlag(QGraphicsItem::ItemIsMovable, true);
   scene->addItem(brod2);

   brod3 = new Ship(QString("Submarine"), 4);
   brod3->setFlag(QGraphicsItem::ItemIsSelectable, true);
   brod3->setFlag(QGraphicsItem::ItemIsMovable, true);
   scene->addItem(brod3);

   brod4 = new Ship(QString("Submarine"), 5);
   brod4->setFlag(QGraphicsItem::ItemIsSelectable, true);
   brod4->setFlag(QGraphicsItem::ItemIsMovable, true);
   scene->addItem(brod4);
}

Main::~Main()
{
    delete ui;
}


