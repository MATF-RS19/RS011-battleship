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
#include "Player.h"
#include <QList>


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
                  Qt::KeepAspectRatioByExpanding); // This scales the image too tall
   QImage sizedImage = QImage(sized.toImage());
   QImage sizedCroppedImage = QImage(sizedImage.copy(0,0,
             (ui->graphicsView->width() - 1),
             (ui->graphicsView->height() - 1)));

   QGraphicsPixmapItem *sizedBackground = scene->addPixmap(
              QPixmap::fromImage(sizedCroppedImage));
    Q_UNUSED(sizedBackground);
    ui->graphicsView->setScene(this->scene);
    ui->graphicsView->setLineWidth(Qt::AlignTop | Qt::AlignLeft);

}
void Main::moveShip(const QPointF startPoint, int length, bool isVertical) const
{
    std::vector<Ship*> listShips = m_p->getShips();
    Ship *ship;
    for(Ship* s : listShips){
        if(s->getShipLength() == length){
            ship = s;
        }
    }
    if(isVertical){
        if(ship->getIsVertical())//radi
            ship->setPos(startPoint.x()-16-20*ship->getShipLength()-220, startPoint.y()-100);
        else{
            //rotate
            //qDebug() << "Ovdeeeeeeeeeeeeeeee";
            ship->rotationShip();
            ship->setIsVertical(1);
            ship->setPos(startPoint.x()-16-20*ship->getShipLength()-220, startPoint.y()-100);
        }

    }
    else{
        if(!ship->getIsVertical()){//radi
            //pomeranje
            ship->setPos(startPoint.x()-6-20*ship->getShipLength()-220, startPoint.y()-110);
        }else{
            //rotation
            //qDebug() << "2.2.";
            ship->rotationShip();
            ship->setIsVertical(0);
            ship->setPos(startPoint.x()-16-20*ship->getShipLength()-220+ship->getShipLength()*5, startPoint.y()-110-(ship->getShipLength()-2)*5);

        }
         //ship->setPos(position);
    }
}

void Main::pickedSquare(Square *square)
{
    if(!pSquare){
        pSquare = square;
    }
    else if(pSquare && !kSquare){
        //qDebug() << "preeee";
        //board1->clickedOn(pSquare->getI(), pSquare->getJ(), kSquare->getI(), kSquare->getJ());
        //qDebug() << "poslee";
        kSquare = square;
        bool val1 =  pSquare->getJ() == kSquare->getJ();
        bool val2 = pSquare->getI() == kSquare->getI();
        int l;
        if(pSquare->getI() - kSquare->getI()){
            l = pSquare->getI() - kSquare->getI();
        }
        else{
            l = pSquare->getJ() - kSquare->getJ();
        }
        l = std::abs(l)+1;
        //qDebug() << "Izbacuje " << l;
        //qDebug() << "IsVer " << val1;
        if(!m_lock && (val1 || val2))
            moveShip(pSquare->pos(),l,val1);
    }
    else{
        pSquare = square;
        kSquare = nullptr;
    }
    qDebug() << "Za novo" << square->pos();
}
void Main::start()
{
    m_p = new Player("Alex");
    Board *squareBoard1 = new Board();
    Board *squareBoard2 = new Board();
    QGraphicsScene *scene = ui->graphicsView->scene();
    squareBoard1->placeSquares(70, 100, 10, 10, true, scene);
    squareBoard2->placeSquares(370, 100, 10, 10, false, scene);
    m_p->init_ships();
    auto vec = m_p->getShips();
    for(auto s : vec){
        scene->addItem(s);
        s->setRect(ui->graphicsView->scene()->sceneRect());
        s->setAnchorPoint(s->pos());
    }
}

Main::~Main()
{
    delete ui;
}



void Main::on_Ready_clicked()
{
      m_lock=1;
}
