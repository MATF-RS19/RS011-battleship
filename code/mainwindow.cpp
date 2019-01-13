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

QEventLoop loop;

bool game_over = false;
bool players_turn = true;
bool machines_turn = false;
Square *currentSquare;

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

   /*
   When the player clicks on Ready button, the squares on the opponent's board are connected to
   the function that processes player's attack.
   */
   connectSquaresToPlayersAttackFunction();

   // The game begins
   game();

}

// The squares on opponent's board are connected to the function playersAttack
void Main::connectSquaresToPlayersAttackFunction() {
    auto squares = m_board2->getSquares();
    for (auto square : squares) {
        connect(square, SIGNAL(squareClicked(int, int, QPointF)), this, SLOT(playersAttack(int, int, QPointF)));
    }

}

// Player's actions no longer have any effect on the game
void Main::disconnectSquaresToPlayersAttackFunction() {
    auto squares = m_board2->getSquares();
    for (auto square : squares) {
        disconnect(square, SIGNAL(squareClicked(int, int, QPointF)), this, SLOT(playersAttack(int, int, QPointF)));
    }
}

void Main::game()
{
    while(!game_over) {
        if (players_turn) {
            /*
             * The loop waits upon a signal, we exit the loop when playersAttack function is called,
             * actually, when the player clicks on some square
            */
            loop.exec();
        }

        if (machines_turn) {
            machinesAttack();
        }
    }
}

void Main::playersAttack(int i, int j, QPointF position) {

    /*
     * If the position where the player clicked has the value 1 in the matrix of the machine
     * then the player guessed correctly.
     */
    if (m_machine->getValueOnPosition(i, j) == 1) {
        // The number of player's correct guesses increases
        int correct = m_player->getCorrectGuesses();
        m_player->setCorrectGuesses(correct + 1);

        // Green squares are drawn
        displayClickOnSquare(i, j, true, "PLAYER");

        // If the player won
        if (m_player->IWon()) {

            // The game is stopped
            players_turn = false;
            game_over = true;

            // Player's actions no longer have any effect on the game
            disconnectSquaresToPlayersAttackFunction();

            // The message is written on the screen
            gameOverDesign("YOU WON!");

        }
    } else {
        // If the player guessed wrong, red square is drawn on the board, and it's machine's turn to play now
        displayClickOnSquare(i, j, false, "PLAYER");
        players_turn = false;
        machines_turn = true;
    }
    // We exit if branch in function game
    loop.exit();
}

void Main::machinesAttack() {

    // Random square is chosen to be attacked
    int x = qrand() % 10;
    int y = qrand() % 10;

    QPoint *currentPoint = new QPoint(x, y);

    // If that square is already attacked, find other that is not
    while (m_machine->givenSquareIsAlreadyAttacked(*currentPoint)) {
        x = qrand() % 10;
        y = qrand() % 10;
        currentPoint = new QPoint(x, y);
    }

    // Chosen square is added to already attacked
    std::vector<QPoint> *attackedSquares = m_machine->getAttackedSquares();
    attackedSquares->push_back(*currentPoint);
    m_machine->setAttackedSquares(*attackedSquares);

    // We take the squares from player's board
    auto sq1 = m_board1->getSquares();

    for (auto s : sq1) {
        // If the randomly chosen square corresponds to some square on the player's board
        if (s->getI() == x && s->getJ() == y){
            currentSquare = s;
            break;
        }
    }

    // If that square contains a part of the ship
    if (currentSquare->getSelected()) {
        // The number of machine's correct guesses increases
        int correct = m_machine->getCorrectGuesses();
        m_machine->setCorrectGuesses(correct + 1);

        // Green squares are drawn
        displayClickOnSquare(x, y, true, "MACHINE");

        // If machine won, the game is stopped, and corresponding message is displayed
        if (m_machine->IWon()) {

            machines_turn = false;
            game_over = true;

            gameOverDesign("YOU LOST!");
        }

    } else {
        // Red squares are drawn
        displayClickOnSquare(x, y, false, "MACHINE");

        // Now it's player's turn to play
        machines_turn = false;
        players_turn = true;
    }
}

// The function processes how the design of the scene changes when a square is clicked on
void Main::displayClickOnSquare(int x, int y, bool correctGuess, QString whoIsPlaying) {

    int tableXBegining;

    // If it's player's turn the squares are drawn on the second board, otherwise they're drawn on the first board
    if (whoIsPlaying == "PLAYER")
        tableXBegining = 370;
    else if (whoIsPlaying == "MACHINE")
        tableXBegining = 70;

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    rect->setRect(tableXBegining + y*20, 100 + x*20, 20, 20);
    // If the move was a correct guess the square is green, otherwise it's red
    correctGuess ? rect->setBrush(Qt::green) : rect->setBrush(Qt::red);
    scene->addItem(rect);
}

// When the game is over, design of the scene changes and a corresponding message is displayed
void Main::gameOverDesign(QString text) {

    // The game is over, so all of the object on the scene are disabled
    int n = scene->items().size();

    for (int k = 0; k < n; k++) {
        scene->items().at(k)->setEnabled(false);
    }

    // Ready button is disabled
    ui->Ready->setEnabled(false);

    // Black somewhat transparent panel is drawn over the entire scene
    QGraphicsRectItem* panel = new QGraphicsRectItem(0,0,640,450);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    panel->setBrush(brush);
    panel->setOpacity(0.65);
    scene->addItem(panel);

    // White panel is drawn on the center of the scene with message in it
    QGraphicsRectItem* panel2 = new QGraphicsRectItem(170,120,300,200);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);
    panel2->setBrush(brush);
    panel2->setOpacity(0.85);
    scene->addItem(panel2);

    // Corresponding message is displayed
    QGraphicsTextItem* wonText = new QGraphicsTextItem(text);
    wonText->setPos(280, 210);
    scene->addItem(wonText);

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


