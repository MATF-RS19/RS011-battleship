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
   QPixmap *source = new QPixmap(":/background2.jpg");

   ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   QPixmap sized = source->scaled(
               QSize(ui->graphicsView->width(),
                     ui->graphicsView->height()),
               Qt::KeepAspectRatioByExpanding);

   QImage sizedImage = QImage(sized.toImage());
   QImage sizedCroppedImage = QImage(sizedImage.copy(0,0,
          (ui->graphicsView->width() - 1),
          (ui->graphicsView->height() - 1)));

   QGraphicsPixmapItem *sizedBackground = scene->addPixmap(
           QPixmap::fromImage(sizedCroppedImage));
   Q_UNUSED(sizedBackground);

   board1 = new Board(70, 100);
   scene->addItem(board1);

   board2 = new Board(370, 100);
   scene->addItem(board2);

   brod1 = new Ship(QString("Submarine"), 2);
   brod1->setFlag(QGraphicsItem::ItemIsSelectable, true);
   brod1->setFlag(QGraphicsItem::ItemIsMovable, true);
   scene->addItem(brod1);
   brod1->acceptedMouseButtons();

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

   ui->graphicsView->setScene(this->scene);
   ui->graphicsView->setLineWidth(Qt::AlignTop | Qt::AlignLeft);

}

Main::~Main()
{
    delete ui;
}


