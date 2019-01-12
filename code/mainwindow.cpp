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
#include "player.h"
#include <QList>
#include <QTest>
#include <QEventLoop>

Square *currentSquare;
QEventLoop loop;

Main::Main(QWidget *parent) :
    QMainWindow(parent),
     ui(new Ui::Main)
//background setting
{
   ui->setupUi(this);

   //adding of "close" option in menu
   ui->meni->addMenu(ui->file);
   ui->file->addAction(ui->quit);
   connect(ui->quit, SIGNAL(triggered()), this, SLOT(close()));
   connect(ui->restart, SIGNAL(triggered()), this, SLOT(restart()));

   scene = new QGraphicsScene(this);
   QPixmap *source = new QPixmap(":/background2.jpg");
   ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

   QPixmap sized = source->scaled(QSize(ui->graphicsView->width(),ui->graphicsView->height()),Qt::KeepAspectRatioByExpanding); // This scales the image too tall
   QImage sizedImage = QImage(sized.toImage());
   QImage sizedCroppedImage = QImage(sizedImage.copy(0,0,(ui->graphicsView->width() - 1),(ui->graphicsView->height() - 1)));
   QGraphicsPixmapItem *sizedBackground = scene->addPixmap(QPixmap::fromImage(sizedCroppedImage));
   Q_UNUSED(sizedBackground);
   ui->graphicsView->setScene(this->scene);
   ui->graphicsView->setLineWidth(Qt::AlignTop | Qt::AlignLeft);
}
//the function for moving and rotating the ship
void Main::moveShip(const QPointF startPoint, int length, bool isVerticalSetting, std::vector<int> coords) const
{
    std::vector<Ship*> listShips = m_player->getShips();
    Ship *ship;
    for(Ship* s : listShips){
        if(s->getShipLength() == length){//finding a ship that needs to be moved
            ship = s;
            ship->setCoordinates(coords);
            ship->setPlacedShip();
        }
    }
    if(isVerticalSetting){//for vertical setting
        if(ship->getIsVertical())//when ship is already vertical, only move
            ship->setPos(startPoint.x()-16-20*ship->getShipLength()-220, startPoint.y()-100);
        else{//set ship to be vertical and move
            ship->rotationShip();
            ship->setIsVertical(1);
            ship->setPos(startPoint.x()-16-20*ship->getShipLength()-220, startPoint.y()-100);
        }

    }
    else{//for horizontal setting
        if(!ship->getIsVertical()){//when ship is already horizontal
            ship->setPos(startPoint.x()-6-20*ship->getShipLength()-220+(ship->getShipLength()-2)*10, startPoint.y()-110-(ship->getShipLength()-2)*10);
        }else{//set ship to be horizontal and move
            ship->rotationShip();
            ship->setIsVertical(0);
            ship->setPos(startPoint.x()-16-20*ship->getShipLength()-220+(ship->getShipLength()-1)*10, startPoint.y()-110-(ship->getShipLength()-2)*10);

        }
    }
}
//the function that pickes two squares that will determine ship position
void Main::pickedSquare(Square *square)
{
    if(!startSquare){//first square
        startSquare = square;
    }
    else if(startSquare && !endSquare){//second square, positioning can start
        endSquare = square;

        bool isVerticalSetting =  startSquare->getJ() == endSquare->getJ();
        bool isHorizontalSetting = startSquare->getI() == endSquare->getI();
        int length;

        //finding the length and orientation of the ship
        if(startSquare->getI() - endSquare->getI()){
            length = startSquare->getI() - endSquare->getI();
            if(startSquare->getI()>endSquare->getI()){
                std::swap(startSquare, endSquare);
            }
        }
        else{
            length = startSquare->getJ() - endSquare->getJ();
            if(startSquare->getJ()>endSquare->getJ()){
                std::swap(startSquare, endSquare);
            }
        }
        length = std::abs(length)+1;
        if(!m_lock && (isVerticalSetting || isHorizontalSetting)){//moving the ship
            std::vector<int> coord(4);
            coord[0] = startSquare->getI();
            coord[1] = startSquare->getJ();
            coord[2] = endSquare->getI();
            coord[3] = endSquare->getJ();
            moveShip(startSquare->pos(), length, isVerticalSetting, coord);
        }
    }
    else{//if two squares are already picked, this square will be first square for other ship
        startSquare = square;
        endSquare = nullptr;
    }
}
void Main::init()//initialization of player, boards and machine
{
    m_player = new Player("Alex");
    m_machine = new Machine();
    m_board1 = new Board();
    m_board2 = new Board();
    QGraphicsScene *scene = ui->graphicsView->scene();
    m_board1->placeSquares(70, 100, 10, 10, true, scene);
    m_board2->placeSquares(370, 100, 10, 10, false, scene);
    m_player->init_ships();
    auto ships = m_player->getShips();
    for(auto s : ships){
        scene->addItem(s);
        s->setRect(ui->graphicsView->scene()->sceneRect());
        s->setAnchorPoint(s->pos());
    }

}

Main::~Main()
{
    delete ui;
}



void Main::on_Ready_clicked()//clicked when game is ready to start
{

    std::vector<Ship*> ships = m_player->getShips();
    bool allPlaced = true;
    bool withoutOverlap = true;
    for (Ship* s: ships) {//takes the final position of the ships
        if(s->getPlacedShip()){
            std::vector<int> coord = s->getCoordinates();
            if(!m_board1->selectedSquares(coord[0], coord[1], coord[2], coord[3])){
                withoutOverlap = false;
            }
        }
        else{
            allPlaced = false;
        }
    }
   m_lock=1;//locks the positions of ships

}
void Main::deinit(){

    delete m_player;
    delete m_machine;
    delete m_board1;
    delete m_board2;
}
void Main::restart(){

    deinit();
    m_lock = 0;
    startSquare = nullptr;
    endSquare = nullptr;
    init();
    this->show();
}
bool Main::getLock() const
{
    return m_lock;
}
