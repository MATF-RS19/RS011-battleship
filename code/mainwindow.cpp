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
#include <QJsonDocument>

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
   ui->file->addAction(ui->restart);
   ui->file->addAction(ui->quit);

   ui->numbers1->setText(" 1\n 2\n 3\n 4\n 5\n 6\n 7\n 8\n 9\n10");
   ui->numbers2->setText(" 1\n 2\n 3\n 4\n 5\n 6\n 7\n 8\n 9\n10");
   ui->alfabet1->setText("  A   B   C   D   E   F   G   H    I    J");
   ui->alfabet2->setText("  A   B   C   D   E   F   G   H    I    J");

   connect(ui->ready, SIGNAL(clicked(bool)), this, SLOT(onReadyClicked()));

   drawScene();
}

Main::~Main()
{
    delete ui;
}

bool Main::connectToGameServer(QString name, QHostAddress address, int port)
{
    m_mode = 2;
    m_socket = new QTcpSocket(nullptr);
    m_player = new Player(m_name);
    m_name = name;
    m_opponent = "Player 2";
    this->setWindowTitle(m_name);
    ui->l1->setText(m_name);
    ui->l2->setText(m_opponent);
    ui->restart->setVisible(false);

    connect(ui->quit, SIGNAL(triggered()), this, SLOT(closeForTwoPlayers()));

    drawBoards();
    drawShips();

    m_socket->connectToHost(address, port);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(processMessage()));

    qDebug() << "Connected to server, I am client " << m_name;
    return m_socket->waitForConnected(30000);
}

void Main::singlePlayer(QString name, QString opponent)
{
    m_name = name;
    m_opponent = opponent;
    m_mode = 1;
    ui->file->addAction(ui->restart);
    connect(ui->restart, SIGNAL(triggered()), this, SLOT(restart()));
    connect(ui->quit, SIGNAL(triggered()), this, SLOT(close()));

    m_player = new Player(m_name);
    m_machine = new Machine();
    this->setWindowTitle(m_name);
    ui->l1->setText(m_name);
    ui->l2->setText(m_opponent);

    drawBoards();
    drawShips();
}

void Main::drawScene()
{
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

    QGraphicsRectItem *r1 = new QGraphicsRectItem();
    r1->setRect(59, 99, 201, 201);
    scene->addItem(r1);
    QGraphicsRectItem *r2 = new QGraphicsRectItem();
    r2->setRect(389, 99, 201, 201);
    scene->addItem(r2);
}

void Main::drawBoards()
{
    m_board1 = new Board();
    m_board2 = new Board();
    m_board1->setNamePlayer(m_name);
    m_board2->setNamePlayer(m_opponent);
    m_board1->placeSquares(60, 100, 10, 10, true, scene);
    m_board2->placeSquares(390, 100, 10, 10, false, scene);
}

void Main::drawShips()
{
    QGraphicsScene *sc = ui->graphicsView->scene();
    m_player->init_ships();
    auto vec = m_player->getShips();
    for(auto s : vec){
        sc->addItem(s);
        s->setRect(ui->graphicsView->scene()->sceneRect());
        s->setAnchorPoint(s->pos());
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
void Main::gameOverDesign(QString text)
{
    m_finished = true;
    // The game is over, so all of the object on the scene are disabled
    int n = scene->items().size();

    for (int k = 0; k < n; k++) {
        scene->items().at(k)->setEnabled(false);
    }

    // Ready button is disabled
    ui->ready->setEnabled(false);

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

void Main::deinit()
{
    delete m_player;
    delete m_machine;
    delete m_board1;
    delete m_board2;
}

// The squares on opponent's board are connected to the function playersAttack
void Main::connectSquaresToPlayersAttackFunction()
{
    auto squares = m_board2->getSquares();
    for (auto square : squares) {
        connect(square, SIGNAL(squareClicked(int, int, QPointF)), this, SLOT(playersAttack(int, int, QPointF)));
    }

}

// Player's actions no longer have any effect on the game
void Main::disconnectSquaresToPlayersAttackFunction()
{
    auto squares = m_board2->getSquares();
    for (auto square : squares) {
        disconnect(square, SIGNAL(squareClicked(int, int, QPointF)), this, SLOT(playersAttack(int, int, QPointF)));
    }
}

void Main::enableMove()
{
    qDebug() << "move " << m_name;
    auto squares = m_board2->getSquares();
    for (auto square : squares) {
        connect(square, SIGNAL(squareClicked(int, int, QPointF)), this, SLOT(playMove(int, int, QPointF)));
    }
    return;
}

void Main::disableMove()
{
    qDebug() << "end move " << m_name;
    auto squares = m_board2->getSquares();
    for (auto square : squares) {
        disconnect(square, SIGNAL(squareClicked(int, int, QPointF)), this, SLOT(playMove(int, int, QPointF)));
    }
    return;
}

bool Main::getLock() const
{
    return m_lock;
}

void Main::onReadyClicked()//clicked when game is ready to start
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
    if (allPlaced) {
        m_lock=1;//locks the positions of ships
        ui->ready->hide();

        if (m_mode == 2) {
            m_obj.insert("name", m_name);
            m_obj.insert("ready", m_name);
            QJsonDocument doc(m_obj);
            m_socket->write(doc.toJson());
        } else if (m_mode == 1) {
            /*
             * When the player clicks on Ready button, the squares on the opponent's board are connected to
             * the function that processes player's attack.
             */
            connectSquaresToPlayersAttackFunction();


            // The game begins
            game();
        }
    }

}

void Main::restart()
{
    startSquare = nullptr;
    endSquare = nullptr;
    if (m_finished == true) {
        ui->numbers1->show();
        ui->numbers2->show();
        ui->alfabet1->show();
        ui->alfabet2->show();
    }
    deinit();
    m_finished = false;
    if (m_lock == 1) {
        ui->ready->setEnabled(true);
        ui->ready->show();
    }
    m_lock = 0;
    scene->clear();
    m_player = new Player(m_name);
    m_machine = new Machine();
    drawScene();
    drawBoards();
    drawShips();
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
            disableMove();

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

void Main::playMove(int i, int j, QPointF position)
{
    qDebug() << "playMove" << m_obj << m_name;
    if (m_clicked == false) {
        m_obj.insert("i", i);
        m_obj.insert("j", j);
        m_obj.insert("posx", position.x());
        m_obj.insert("posy", position.y());
        m_obj.insert("check", m_obj.value("opponent"));
        m_clicked = true;
        QJsonDocument o(m_obj);
        m_socket->write(o.toJson());
    }
    qDebug() << "end playMove" << m_obj;
}

void Main::processMessage()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QJsonDocument msg = QJsonDocument::fromJson(socket->readAll());
    m_obj = msg.object();
    qDebug() << "Client processMessage:" << m_obj;

    if (m_obj.contains("wait")) {
        QJsonDocument o(m_obj);
        m_socket->write(o.toJson());
        return;
    }
    if (m_obj.contains("startgame")) {
        messageStartGame();
        return;
    }
    if (m_obj.contains("check")) {
        messageCheck();
        return;
    }
    if (m_obj.contains("turn") && !m_obj.contains("winner")) {
        messageTurn();
        return;
    }
    if (m_obj.contains("display")) {
        messageDisplay();
        return;
    }
    if (m_obj.contains("endgame")) {
        messageEndGame();
        return;
    }
    if (m_obj.contains("quit")) {
        messageQuit();
        return;
    }

    qDebug() << "end processMessage";
}

void Main::closeForTwoPlayers()
{
    QJsonObject tmp;
    tmp.insert("quit", m_name);
    QJsonDocument o(tmp);
    m_socket->write(o.toJson());
    disconnect(m_socket, SIGNAL(aboutToClose()), this, SLOT(close()));
    this->close();
}

bool Main::checkBoard(int i, int j, Board *board)
{
    auto temp = board->getSquares();
    for (auto t: temp) {
        if (t->getI() == i && t->getJ() == j) {
            if (t->getSelected() == true) {
                m_countSinked++;
                return true;
            }
        }
    }
    return false;
}

void Main::findPositionOfOpponentsAttack(int i, int j)
{
    auto board = m_board1->getSquares();
    for (auto s : board)
        if (s->getI() == i && s->getJ() == j) {
            m_posAttack = s->pos();
        }
}

void Main::messageStartGame()
{
    qDebug() << "startgame" << m_obj;
    if (m_obj.value("startgame").toString() == m_name) {
        m_obj.insert("turn", m_name);
    }
    m_obj.remove("startgame");
    QJsonDocument o(m_obj);
    m_socket->write(o.toJson());
}

void Main::messageTurn()
{
    qDebug() << "turn " << m_obj << m_name;
    if (m_obj.value("turn").toString() == m_name) {
        ui->label->setText("It's your turn to play " + m_name +"!");
        enableMove();
        return;
    }
    if (m_obj.value("turn").toString() != m_name) {
        ui->label->setText("It's opponents turn to play!");
        if (m_clicked == false)
            m_clicked = true;
        disableMove();
        return;
    }
}

void Main::messageCheck()
{
    qDebug() << "check " << m_obj << m_name;
    disableMove();
    m_clicked = false;
    if (m_obj.value("check").toString() == m_name) {
        int i = m_obj.value("i").toInt();
        int j = m_obj.value("j").toInt();
        QPointF pos(m_obj.value("posx").toInt(), m_obj.value("posy").toInt());
        qDebug() << i << j << pos;
        bool t = checkBoard(i, j, m_board1);
        if (m_countSinked == 14) {
            m_obj.insert("sinked", 14);
        }
        m_obj.insert("hit", t);
        m_obj.insert("ph", m_obj.value("turn").toString());
        qDebug() << "check" << m_name << m_obj;
        QJsonDocument o(m_obj);
        m_socket->write(o.toJson());
    }
}

void Main::messageDisplay()
{
    qDebug() << "display " << m_obj;
    if (m_obj.value("display").toString() == m_name) {
        qDebug() << "display" << m_obj;
        QPointF pos(m_obj.value("posx").toInt(), m_obj.value("posy").toInt());
        bool t = m_obj.value("value").toBool();
//        displayClickOnSquare(t, pos);
        if (m_obj.contains("winner")) {
            m_obj.insert("endgame", m_obj.value("winner").toString());
        } else {
            if (t == true)
                m_obj.insert("turn", m_obj.value("now").toString());
            else {
                m_obj.insert("turn", m_obj.value("next").toString());
                m_obj.insert("opponent", m_obj.value("now").toString());
            }
            m_obj.insert("previous", m_obj.value("now").toString());
            if (m_obj.value("previous").toString() == m_obj.value("turn").toString())
                m_clicked = false;
        }
        m_obj.remove("display");
        m_obj.remove("posx");
        m_obj.remove("i");
        m_obj.remove("j");
        m_obj.remove("posy");
        m_obj.remove("value");
        QJsonDocument o(m_obj);
        m_socket->write(o.toJson());
        qDebug() << "end display" << m_obj;
    } else if (m_obj.value("display").toString() != m_name) {
        int i = m_obj.value("i").toInt();
        int j = m_obj.value("j").toInt();
        findPositionOfOpponentsAttack(i, j);
        QPointF pos(m_posAttack);
        bool t = m_obj.value("value").toBool();

    }

}

void Main::messageEndGame()
{
    QJsonObject end;
    if (m_obj.value("winner").toString() == m_name) {
        gameOverDesign("YOU WON!");
        end.insert("winnerend", true);
    } else if (m_obj.value("loser").toString() == m_name) {
        gameOverDesign("YOU LOST!");
        end.insert("loserend", true);
    }
    end.insert("done", true);
    QJsonDocument o(end);
    m_socket->write(o.toJson());
}

void Main::messageQuit()
{
    if (m_obj.value("quit") != m_name) {
        gameOverDesign("YOU WON!");
        m_obj.insert("done", true);
        QJsonDocument o(m_obj);
        m_socket->write(o.toJson());
    }
}

