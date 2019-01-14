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
Square *originalSquare;

std::vector<QPoint> nextTrys;
QPoint firstGuess;

QPoint newGuess;

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

//  When button "Two Players" is clicked we need to connect our socket
//  to the server with IP address that we collect from lineEdit in dialog.
//  We initialize player, boards and ships, and we connect our socket
//  to slot processMessage whenever signal readyRead() is emited.
bool Main::connectToGameServer(QString name, QHostAddress address, int port)
{
    m_mode = 2;
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

//  When button "Single Player" is clicked in dialog we initialize
//  player and machine, boards and ships, and connect button with
//  slots.
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

    m_socket = new QTcpSocket(nullptr);

    QGraphicsRectItem *r1 = new QGraphicsRectItem();
    r1->setRect(59, 99, 201, 201);
    scene->addItem(r1);
    QGraphicsRectItem *r2 = new QGraphicsRectItem();
    r2->setRect(389, 99, 201, 201);
    scene->addItem(r2);
}

//  Initializing of the boards.
void Main::drawBoards()
{
    m_board1 = new Board();
    m_board2 = new Board();
    m_board1->setNamePlayer(m_name);
    m_board2->setNamePlayer(m_opponent);
    m_board1->placeSquares(60, 100, 10, 10, true, scene);
    m_board2->placeSquares(390, 100, 10, 10, false, scene);
}

//  Initializing ships for the player and adding them to the scene.
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

// Function processes machine's attack
void Main::machinesAttack() {

    int x, y;
    QPoint *currentPoint;

    // if the machine doesn't have next square to attack
    if (!m_machine->getHaveNewGuesses()) {
        // machine chooses random square
        x = qrand() % 10;
        y = qrand() % 10;

        currentPoint = new QPoint(x, y);

        // if that square is already attacked, find one that isn't
        while (m_machine->givenSquareIsAlreadyAttacked(*currentPoint)) {
            x = qrand() % 10;
            y = qrand() % 10;
            currentPoint = new QPoint(x, y);
        }

        // Chosen square is added to already attacked
        std::vector<QPoint> *attackedSquares = m_machine->getAttackedSquares();
        attackedSquares->push_back(*currentPoint);
        m_machine->setAttackedSquares(*attackedSquares);
    } else {
        // if the machine has next square to attack
        x = newGuess.x();
        y = newGuess.y();

        // if that square is already attacked, find one that isn't
        while (m_machine->givenSquareIsAlreadyAttacked(newGuess)) {

            // if the machine hasn't found the direction od the ship, take the next square from the list of next trys
            if (!m_machine->getGoUp() && !m_machine->getGoDown() && !m_machine->getGoLeft() && !m_machine->getGoRight()) {

                int n = nextTrys.size();
                if (n > 0) {
                    newGuess = nextTrys.at(n-1);
                    x = newGuess.x();
                    y = newGuess.y();
                    nextTrys.pop_back();
                } else {
                    // if the list is empty, the machines doesn't have new guesses, now it randomly picks squares
                    m_machine->setHaveNewGuesses(false);
                    m_machine->setIsFirstGuessInARow(false);

                    x = qrand() % 10;
                    y = qrand() % 10;

                    QPoint *p = new QPoint(x, y);
                    newGuess = *p;
                }
              // if the direction of the ship was left-right, and machine was moving to the right
            } else if (m_machine->getGoRight()) {
                // stop the machine from going right any further
                m_machine->setGoRight(false);
                // machine went rignt
                m_machine->setWentRight(true);
                // if the machine didn't go left and the original square isn't left border, top left corner or bottom left corner
                if (!m_machine->getWentLeft() && !originalSquare->checkIfSquareIs("leftBorder") && !originalSquare->checkIfSquareIs("topLeftCorner") && !originalSquare->checkIfSquareIs("bottomLeftCorner")) {
                    // set the machine to go left from the original point
                    m_machine->setGoLeft(true);
                    newGuess.setX(firstGuess.x()-1);
                    newGuess.setY(firstGuess.y());
                    x = newGuess.x();
                    y = newGuess.y();
                } else {
                    // if the machine went left, empty the list of next trys and pick a random square
                    int n = nextTrys.size();
                    while (n != 0) {
                        nextTrys.pop_back();
                        n--;
                    }
                    m_machine->setHaveNewGuesses(false);
                    m_machine->setIsFirstGuessInARow(false);

                    x = qrand() % 10;
                    y = qrand() % 10;

                    QPoint *p = new QPoint(x, y);
                    newGuess = *p;
                }
                // if the direction of the ship was left-right, and machine was moving to the left
            } else if (m_machine->getGoLeft()) {
                // stop the machine from going left any further
                m_machine->setGoLeft(false);
                // machine went left
                m_machine->setWentLeft(true);
                // if the machine didn't go right and the original square isn't right border, top right corner or bottom right corner
                if (!m_machine->getWentRight() && !originalSquare->checkIfSquareIs("rightBorder") && !originalSquare->checkIfSquareIs("topRightCorner") && !originalSquare->checkIfSquareIs("bottomRightCorner")) {
                    // set the machine to go right from the original point
                    m_machine->setGoRight(true);
                    newGuess.setX(firstGuess.x()+1);
                    newGuess.setY(firstGuess.y());
                    x = newGuess.x();
                    y = newGuess.y();
                } else {
                    // if the machine went right, empty the list of next trys and pick a random square
                    int n = nextTrys.size();
                    while (n != 0) {
                        nextTrys.pop_back();
                        n--;
                    }
                    m_machine->setHaveNewGuesses(false);
                    m_machine->setIsFirstGuessInARow(false);

                    x = qrand() % 10;
                    y = qrand() % 10;

                    QPoint *p = new QPoint(x, y);
                    newGuess = *p;
                }
                // if the direction of the ship was up-down, and machine was moving up
            } else if (m_machine->getGoUp()) {
                // stop the machine from going up any further
                m_machine->setGoUp(false);
                // machine went up
                m_machine->setWentUp(true);
                // if the machine didn't go down and the original square isn't bottom border, bottom left corner or bottom right corner
                if (!m_machine->getWentDown() && !originalSquare->checkIfSquareIs("bottomBorder") && !originalSquare->checkIfSquareIs("bottomLeftCorner") && !originalSquare->checkIfSquareIs("bottomRightCorner")) {
                    // set the machine to go down from the original point
                    m_machine->setGoDown(true);
                    newGuess.setX(firstGuess.x());
                    newGuess.setY(firstGuess.y()+1);
                    x = newGuess.x();
                    y = newGuess.y();
                } else {
                    // if the machine went down, empty the list of next trys and pick a random square
                    int n = nextTrys.size();
                    while (n != 0) {
                        nextTrys.pop_back();
                        n--;
                    }
                    m_machine->setHaveNewGuesses(false);
                    m_machine->setIsFirstGuessInARow(false);

                    x = qrand() % 10;
                    y = qrand() % 10;

                    QPoint *p = new QPoint(x, y);
                    newGuess = *p;
                }
                // if the direction of the ship was up-down, and machine was moving down
            } else if (m_machine->getGoDown()) {
                // stop the machine from going down any further
                m_machine->setGoDown(false);
                // machine went down
                m_machine->setWentDown(true);
                // if the machine didn't go up and the original square isn't top border, top left corner or top right corner
                if (!m_machine->getWentUp() && !originalSquare->checkIfSquareIs("topBorder") && !originalSquare->checkIfSquareIs("topLeftCorner") && !originalSquare->checkIfSquareIs("topRightCorner")) {
                    // set the machine to go up from the original point
                    m_machine->setGoUp(true);
                    newGuess.setX(firstGuess.x());
                    newGuess.setY(firstGuess.y()-1);
                    x = newGuess.x();
                    y = newGuess.y();
                } else {
                    // if the machine went up, empty the list of next trys and pick a random square
                    int n = nextTrys.size();
                    while (n != 0) {
                        nextTrys.pop_back();
                        n--;
                    }
                    m_machine->setHaveNewGuesses(false);
                    m_machine->setIsFirstGuessInARow(false);

                    x = qrand() % 10;
                    y = qrand() % 10;

                    QPoint *p = new QPoint(x, y);
                    newGuess = *p;
                }
            }
        }

        // Add the chosen square to the list of already attacked squares
        std::vector<QPoint> *attackedSquares = m_machine->getAttackedSquares();
        attackedSquares->push_back(newGuess);
        m_machine->setAttackedSquares(*attackedSquares);
    }
    // Machine takes the sqaures from player's board
    auto sq1 = m_board1->getSquares();

    for (auto s : sq1) {
        // If the chosen square corresponds to the square on the player's board
        if (s->getJ() == x && s->getI() == y) {
            currentSquare = s;
            break;
        }
    }

    // If the chosen square contains a part of the ship
    if (currentSquare->getSelected()) {
        // The machine guessed correctly
        machineGuessedCorrectly(x, y, currentPoint);

    } else {
        // The machine guessed wrong
        machineGuessedWrong(x, y);
    }
}

void Main::fillNextTrys(bool setUp, bool setDown, bool setLeft, bool setRight, int x, int y)
{
    QPoint *pointUp = new QPoint(x, y-1);
    QPoint *pointLeft = new QPoint(x-1, y);
    QPoint *pointDown = new QPoint(x, y+1);
    QPoint *pointRight = new QPoint(x+1, y);

    if (setUp) {
        nextTrys.push_back(*pointUp);
    }

    if (setDown) {
        nextTrys.push_back(*pointDown);
    }

    if (setLeft) {
        nextTrys.push_back(*pointLeft);
    }

    if (setRight) {
        nextTrys.push_back(*pointRight);
    }
}

void Main::newGuessIsTheLastElementFromNextTrys() {
    int n = nextTrys.size();
    newGuess = nextTrys.at(n-1);
    nextTrys.pop_back();
}

// The function processes correct guess by machine
void Main::machineGuessedCorrectly(int x, int y, QPoint *currentPoint) {

    // The number of correct guesses is increased
    int correct = m_machine->getCorrectGuesses();
    m_machine->setCorrectGuesses(correct + 1);

    // If this correct guess is a first correct guess in a row
    if (!m_machine->getIsFirstGuessInARow()) {

        // Machine has a first guess in a row
        m_machine->setIsFirstGuessInARow(true);
        firstGuess = *currentPoint;
        // Original square is the first correct guess in a row
        originalSquare = currentSquare;

        // Machine has new guesses
        m_machine->setHaveNewGuesses(true);

        // Set all the machines current or past movement to false
        m_machine->setGoUp(false);
        m_machine->setGoDown(false);
        m_machine->setGoLeft(false);
        m_machine->setGoRight(false);

        m_machine->setWentUp(false);
        m_machine->setWentDown(false);
        m_machine->setWentLeft(false);
        m_machine->setWentRight(false);

        // If the current square isn't on border
        if (!currentSquare->squareIsOnBorder()) {
            // We fill the list of next trys with points up, down, left and right from the original point
            fillNextTrys(true, true, true, true, x, y);

            // Machine takes the last element from the list and sets it as a new guess
            newGuessIsTheLastElementFromNextTrys();
            // If the current square is bottom right corner
        } else if (currentSquare->checkIfSquareIs("bottomRightCorner")) {
            fillNextTrys(true, false, true, false, x, y);
            newGuessIsTheLastElementFromNextTrys();
        } else if (currentSquare->checkIfSquareIs("topRightCorner")) {

            fillNextTrys(false, true, true, false, x, y);
            newGuessIsTheLastElementFromNextTrys();
        } else if (currentSquare->checkIfSquareIs("rightBorder")) {

            fillNextTrys(true, true, true, false, x, y);
            newGuessIsTheLastElementFromNextTrys();
        } else if (currentSquare->checkIfSquareIs("bottomLeftCorner")) {

            fillNextTrys(true, false, false, true, x, y);
            newGuessIsTheLastElementFromNextTrys();
        } else if (currentSquare->checkIfSquareIs("topLeftCorner")) {
            fillNextTrys(false, true, false, true, x, y);
            newGuessIsTheLastElementFromNextTrys();
        } else if (currentSquare->checkIfSquareIs("leftBorder")) {

            fillNextTrys(true, true, false, true, x, y);
            newGuessIsTheLastElementFromNextTrys();
        } else if (currentSquare->checkIfSquareIs("bottomBorder")) {
            fillNextTrys(true, false, true, true, x, y);
            newGuessIsTheLastElementFromNextTrys();
        } else if (currentSquare->checkIfSquareIs("topBorder")) {
            fillNextTrys(false, true, true, true, x, y);
            newGuessIsTheLastElementFromNextTrys();
        }
    // If the machine has a first guess in a row, but hasn't established in which direction to go
    } else if (m_machine->getIsFirstGuessInARow() && !m_machine->getGoUp() && !m_machine->getGoDown() && !m_machine->getGoLeft() && !m_machine->getGoRight()){
        // if the original square isn't on border
        if (!originalSquare->squareIsOnBorder()) {
            int n = nextTrys.size();
            if (n == 3) {
                m_machine->setGoRight(true);
            } else if (n == 2) {
                m_machine->setGoLeft(true);
            } else if (n == 1) {
                m_machine->setGoDown(true);
            } else if (n == 0) {
                m_machine->setGoUp(true);
            }
            // if the original square is on top border
        } else if (originalSquare->checkIfSquareIs("topBorder")) {
            int n = nextTrys.size();

            if (n == 2) {
                m_machine->setGoRight(true);
            } else if (n == 1) {
                m_machine->setGoLeft(true);
            } else if (n == 0) {
                m_machine->setGoDown(true);
            }
           //  if the original square is on bottom border
        } else if (originalSquare->checkIfSquareIs("bottomBorder")) {
            int n = nextTrys.size();

            if (n == 2) {
                m_machine->setGoRight(true);
            } else if (n == 1) {
                m_machine->setGoLeft(true);
            } else if (n == 0) {
                m_machine->setGoUp(true);
            }
            // if the original square is on right border
        } else if (originalSquare->checkIfSquareIs("rightBorder")) {
            int n = nextTrys.size();

            if (n == 2) {
                m_machine->setGoLeft(true);
            } else if (n == 1) {
                m_machine->setGoDown(true);
            } else if (n == 0) {
                m_machine->setGoUp(true);
            }

           // if the original square is on left border
        } else if (originalSquare->checkIfSquareIs("leftBorder")) {
            int n = nextTrys.size();

            if (n == 2) {
                m_machine->setGoRight(true);
            } else if (n == 1) {
                m_machine->setGoDown(true);
            } else if (n == 0) {
                m_machine->setGoUp(true);
            }
           // if the original square is in top left corner
        } else if (originalSquare->checkIfSquareIs("topLeftCorner")) {
            int n = nextTrys.size();

            if (n == 1) {
                m_machine->setGoRight(true);
            } else if (n == 0) {
                m_machine->setGoDown(true);
            }
            // if the original square is in top right corner
        } else if (originalSquare->checkIfSquareIs("topRightCorner")) {
            int n = nextTrys.size();

            if (n == 1) {
                m_machine->setGoLeft(true);
            } else if (n == 0) {
                m_machine->setGoDown(true);
            }
            // if the original square is in bottom left corner
        } else if (originalSquare->checkIfSquareIs("bottomLeftCorner")) {
            int n = nextTrys.size();

            if (n == 1) {
                m_machine->setGoRight(true);
            } else if (n == 0) {
                m_machine->setGoUp(true);
            }
            // if the original square is in bottom right corner
        } else if (originalSquare->checkIfSquareIs("bottomRightCorner")) {
            int n = nextTrys.size();

            if (n == 1) {
                m_machine->setGoLeft(true);
            } else if (n == 0) {
                m_machine->setGoUp(true);
            }

        }
    }

    // if the direction of machine's movement is up
    if (m_machine->getGoUp()) {
        // if the square above the current is valid take that square as the new guess
        if (y-1 >= 0) {
            QPoint *p = new QPoint(x, y-1);
            newGuess = *p;
        } else {
            // if it isn't valid, stop going up, it went up, and check if it went down, if it didn't try going down,
            // else machine doesn't have new guess
            m_machine->setGoUp(false);
            m_machine->setWentUp(true);
            if (!m_machine->getWentDown()) {
                m_machine->setGoDown(true);
                newGuess.setX(firstGuess.x());
                newGuess.setY(firstGuess.y()+1);
            } else {
                m_machine->setHaveNewGuesses(false);
                m_machine->setIsFirstGuessInARow(false);
            }
        }
        // if the direction of machine's movement is down
    } else if (m_machine->getGoDown()) {
        // if the square below the current is valid take that square as the new guess
        if (y+1 < 10) {
            QPoint *p = new QPoint(x, y+1);
            newGuess = *p;
        } else {
            // if it isn't valid, stop going down, it went down, and check if it went up, if it didn't try going up,
            // else machine doesn't have new guess
            m_machine->setGoDown(false);
            m_machine->setWentDown(true);
            if (!m_machine->getWentUp()) {
                m_machine->setGoUp(true);
                newGuess.setX(firstGuess.x());
                newGuess.setY(firstGuess.y()-1);
            } else {
                m_machine->setHaveNewGuesses(false);
                m_machine->setIsFirstGuessInARow(false);
            }
        }
        // if the direction of machine's movement is left
    } else if (m_machine->getGoLeft()) {
        // if the square to the left is valid take that square as the new guess
        if (x-1 >= 0) {
            QPoint *p = new QPoint(x-1, y);
            newGuess = *p;
        } else {
            // if it isn't valid, stop going left, it went left, and check if it went right, if it didn't try going right,
            // else machine doesn't have new guess
            m_machine->setGoLeft(false);
            m_machine->setWentLeft(true);
            if (!m_machine->getWentRight()) {
                m_machine->setGoRight(true);
                newGuess.setX(firstGuess.x()+1);
                newGuess.setY(firstGuess.y());
            } else {
                m_machine->setHaveNewGuesses(false);
                m_machine->setIsFirstGuessInARow(false);
            }
        }
        // if the direction of machine's movement is right
    } else if (m_machine->getGoRight()) {
        // if the square to the right is valid take that square as the new guess
        if (x+1 < 10) {
            QPoint *p = new QPoint(x+1, y);
            newGuess = *p;
        } else {
            // if it isn't valid, stop going right, it went right, and check if it went left, if it didn't try going left,
            // else machine doesn't have new guess
            m_machine->setGoRight(false);
            m_machine->setWentRight(true);
            if (!m_machine->getWentLeft()) {
                m_machine->setGoLeft(true);
                newGuess.setX(firstGuess.x()-1);
                newGuess.setY(firstGuess.y());
            } else {
                m_machine->setHaveNewGuesses(false);
                m_machine->setIsFirstGuessInARow(false);
            }
        }
    }

    // display squares
    QPointF* position = new QPointF(60 + 20*x, 100 + 20*y);
    displayClickOnSquare(true, *position);

    // if the machine guessed correctly delay it's next movement by 1.5 seconds so it would be better visible
    delay();

    // If the machine won, the player lost and game over
    if (m_machine->IWon()) {

        machines_turn = false;
        game_over = true;

        gameOverDesign("YOU LOST!");
    }
}

void Main::machineGuessedWrong(int x, int y)
{
    // display squares
    QPointF* position = new QPointF(60 + 20*x, 100 + 20*y);
    displayClickOnSquare(false, *position);

    // If machine has first guess in a row but it hasn't established the direction of the movement
    if (m_machine->getIsFirstGuessInARow() && !m_machine->getGoUp() && !m_machine->getGoDown() && !m_machine->getGoLeft() && !m_machine->getGoRight()){
        int n = nextTrys.size();
        if (n > 0) {
            // New guess is next square from the list of next trys
            newGuess = nextTrys.at(n-1);
            nextTrys.pop_back();
        } else {
            // Machines doesn't have new guesses
            m_machine->setHaveNewGuesses(false);
            m_machine->setIsFirstGuessInARow(false);
        }
        connectSquaresToPlayersAttackFunction();
        // It's player's turn to play
        machines_turn = false;
        players_turn = true;
    } else if (m_machine->getIsFirstGuessInARow()) {
        // If the machine was moving up and made a mistake
        if (m_machine->getGoUp()) {
            // stop going up, went up, if it didn't go down and original square is valid
            m_machine->setGoUp(false);
            m_machine->setWentUp(true);
            if (!m_machine->getWentDown() && !originalSquare->checkIfSquareIs("bottomLeftCorner") && !originalSquare->checkIfSquareIs("bottomRightCorner") && !originalSquare->checkIfSquareIs("bottomBorder")) {
                m_machine->setGoDown(true);
                newGuess.setX(firstGuess.x());
                newGuess.setY(firstGuess.y()+1);
            } else {
                m_machine->setHaveNewGuesses(false);
                m_machine->setIsFirstGuessInARow(false);
            }
            // If the machine was moving down and made a mistake
        } else if (m_machine->getGoDown()) {
            // stop going down, went down, if it didn't go up and original square is valid
            m_machine->setGoDown(false);
            m_machine->setWentDown(true);
            if (!m_machine->getWentUp() && !originalSquare->checkIfSquareIs("topLeftCorner") && !originalSquare->checkIfSquareIs("topRightCorner") && !originalSquare->checkIfSquareIs("topBorder")) {
                m_machine->setGoUp(true);
                newGuess.setX(firstGuess.x());
                newGuess.setY(firstGuess.y()-1);
            } else {
                m_machine->setHaveNewGuesses(false);
                m_machine->setIsFirstGuessInARow(false);
            }
            // If the machine was moving left and made a mistake
        } else if (m_machine->getGoLeft()) {
            // stop going left, went left, if it didn't go right and original square is valid
            m_machine->setGoLeft(false);
            m_machine->setWentLeft(true);
            if (!m_machine->getWentRight() && !originalSquare->checkIfSquareIs("bottomRightCorner") && !originalSquare->checkIfSquareIs("topRightCorner") && !originalSquare->checkIfSquareIs("rightBorder")) {
                m_machine->setGoRight(true);
                newGuess.setX(firstGuess.x()+1);
                newGuess.setY(firstGuess.y());
            } else {
                m_machine->setHaveNewGuesses(false);
                m_machine->setIsFirstGuessInARow(false);
            }
            // If the machine was moving right and made a mistake
        } else if (m_machine->getGoRight()) {
            // stop going right, went right, if it didn't go left and original square is valid
            m_machine->setGoRight(false);
            m_machine->setWentRight(true);
            if (!m_machine->getWentLeft() && !originalSquare->checkIfSquareIs("bottomLeftCorner") && !originalSquare->checkIfSquareIs("topLeftCorner") && !originalSquare->checkIfSquareIs("leftBorder")) {
                m_machine->setGoLeft(true);
                newGuess.setX(firstGuess.x()-1);
                newGuess.setY(firstGuess.y());
            } else {
                m_machine->setHaveNewGuesses(false);
                m_machine->setIsFirstGuessInARow(false);
            }
        }
        connectSquaresToPlayersAttackFunction();
        // It's player's turn to play
        machines_turn = false;
        players_turn = true;
    } else if (!m_machine->getIsFirstGuessInARow()) {
        // It's player's turn to play
        connectSquaresToPlayersAttackFunction();
        machines_turn = false;
        players_turn = true;
    }

}

// Funtion to delay machines attack after correct guess
void Main::delay()
{
    QTime time = QTime::currentTime().addSecs(1.5);
    while (QTime::currentTime() < time)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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
        if(!m_lock && (isVerticalSetting || isHorizontalSetting) && length>=2 && length<=5){//moving the ship
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
void Main::displayClickOnSquare(bool correctGuess, QPointF position) {

    QGraphicsPixmapItem* picture = new QGraphicsPixmapItem();
    QPixmap pixmap;

    if (correctGuess)
        pixmap = QPixmap(":/fire.png").scaled(QSize(20, 20));
    else
        pixmap = QPixmap(":/miss.png").scaled(QSize(20, 20));

    picture->setPixmap(pixmap);
    picture->setPos(position);
    picture->show();

    scene->addItem(picture);
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

    ui->alfabet1->hide();
    ui->alfabet2->hide();
    ui->numbers1->hide();
    ui->numbers2->hide();
    ui->labelForCorrectPositionedShips->hide();
    ui->label->hide();
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
    panel2->setOpacity(0.95);
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

//  Enables mousePressEvents on board2 which represents
//  board of the oppoenent.
void Main::enableMove()
{
    qDebug() << "move " << m_name;
    auto squares = m_board2->getSquares();
    for (auto square : squares) {
        connect(square, SIGNAL(squareClicked(int, int, QPointF)), this, SLOT(playMove(int, int, QPointF)));
    }
}

//  Disables mousePressEvents on board2 which represents
//  board of the opponent.
void Main::disableMove()
{
    qDebug() << "end move " << m_name;
    auto squares = m_board2->getSquares();
    for (auto square : squares) {
        disconnect(square, SIGNAL(squareClicked(int, int, QPointF)), this, SLOT(playMove(int, int, QPointF)));
    }
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
    if (allPlaced && withoutOverlap) {//all ships are placed and without overlap
        m_lock=1;//locks the positions of ships
        ui->ready->hide();

        ui->labelForCorrectPositionedShips->setText("Ships are set correctly");
        QPalette palette = ui->labelForCorrectPositionedShips->palette();
        palette.setColor(ui->labelForCorrectPositionedShips->foregroundRole(), Qt::green);
        ui->labelForCorrectPositionedShips->setPalette(palette);

        //  If two players are playing then we need to start the communication between
        //  server and two players. We insert players na
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
            qDebug() << "OVDE SE MORALO DESITI KONEKTOVANJE";


            // The game begins
            game();
        }
    }
    else{//not all ships are set or overlap
        m_board1->restartSelected();
        ui->labelForCorrectPositionedShips->setText("Ships are not set correctly");
        QPalette palette = ui->labelForCorrectPositionedShips->palette();
        palette.setColor(ui->labelForCorrectPositionedShips->foregroundRole(), Qt::red);
        ui->labelForCorrectPositionedShips->setPalette(palette);

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
    ui->labelForCorrectPositionedShips->clear();
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

    qDebug() << "Pozvao se playersAttack";

    /*
     * If the position where the player clicked has the value 1 in the matrix of the machine
     * then the player guessed correctly.
     */
    if (m_machine->getValueOnPosition(i, j) == 1) {
        // The number of player's correct guesses increases
        int correct = m_player->getCorrectGuesses();
        m_player->setCorrectGuesses(correct + 1);

        qDebug() << "CORRECT: " << correct + 1;

        // Green squares are drawn
        displayClickOnSquare(true, position);

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
        displayClickOnSquare(false, position);
        players_turn = false;
        machines_turn = true;

        disconnectSquaresToPlayersAttackFunction();
    }
    // We exit if branch in function game
    loop.exit();
}

//  When signal squareClicked(int, int, QPoint) in mousePressEvent is emited
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

//  This function processes each message socket receives and
//  depending on the message appropriate function is called.
void Main::processMessage()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QJsonDocument msg = QJsonDocument::fromJson(socket->readAll());
    m_obj = msg.object();
    qDebug() << "Client processMessage:" << m_obj;

    if (m_obj.contains("wait")) {
        QJsonDocument o(m_obj);
        m_socket->write(o.toJson());
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

//  When quit is clicked in menu bar then we write to socket
//  new message and disconnect our socket.
void Main::closeForTwoPlayers()
{
    QJsonObject tmp;
    tmp.insert("quit", m_name);
    QJsonDocument o(tmp);
    m_socket->write(o.toJson());
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(close()));
    this->close();
}

//  This function checks if square in board which was attacked contains
//  a part of any ship in that square and returns appropriate number.
int Main::checkBoard(int i, int j, Board *board)
{
    //  0 - it was already attacked
    //  when it wasn't already attacked
    //      1 - it was a hit
    //      2 - it was a miss
    auto temp = board->getSquares();
    for (auto t: temp) {
        if (t->getI() == i && t->getJ() == j) {
            if (t->getAttacked() == false) {
                if (t->getSelected() == true) {
                    t->setAttacked();
                    m_countSinked++;
                    return 1;
                }
            } else {
                return 0;
            }
        }
    }
    return 2;
}

//  This function finds position of specific square in players board
//  because we use it to display where opponent attacked.
void Main::findPositionOfOpponentsAttack(int i, int j)
{
    auto board = m_board1->getSquares();
    for (auto s : board)
        if (s->getI() == i && s->getJ() == j) {
            m_posAttack = s->pos();
        }
}

//  If message contains value "startgame" we need to determine who is
//  playing first and to insert new values into QJsonObject and write
//  to the socket.
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

//  If message contains value "turn" then a message is shown in label
//  so that each player knows whos turn it is, and we enable or disable
//  players to click on opponents board depending if it's theirs turn.
void Main::messageTurn()
{
    qDebug() << "turn " << m_obj << m_name;

    if (m_obj.value("turn").toString() == m_name) {
        ui->label->setText("It's your turn to play " + m_name +"!");
        enableMove();
    }
    if (m_obj.value("turn").toString() != m_name) {
        ui->label->setText("It's opponents turn to play!");
        if (m_clicked == false)
            m_clicked = true;
        disableMove();
    }
}

//  If message contains value "check" that means we have values of
//  opponents attack so we check if it was a hit or a miss and we
//  insert new values into the QJsonObject and write it to the socket.
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
        int t = checkBoard(i, j, m_board1);
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

//  If message contains value "display" then we are displaying value
//  of the attack on ours board and on opponents board. Based on the
//  value of the attack we determine who is playing next and insert
//  new values in QJsonObject and write it to the socket. If opponent
//  is a winner then we insert the value "endgame" and write it to
//  the socket.
void Main::messageDisplay()
{
    qDebug() << "display " << m_obj;
    if (m_obj.value("display").toString() == m_name) {
        qDebug() << "display" << m_obj;
        QPointF pos(m_obj.value("posx").toInt(), m_obj.value("posy").toInt());
        int t = m_obj.value("value").toInt();
        if (t != 0) {
            if (t == 1)
                displayClickOnSquare(true, pos);
            else
                displayClickOnSquare(false, pos);
        }
        if (m_obj.contains("winner")) {
            m_obj.insert("endgame", m_obj.value("winner").toString());
        } else {
            if (t == 1)
                m_obj.insert("turn", m_obj.value("now").toString());
            else if (t == 2 || t == 0){
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
        int t = m_obj.value("value").toInt();
        if (t != 0) {
            if (t == 1)
                displayClickOnSquare(true, pos);
            else
                displayClickOnSquare(false, pos);
        }
    }
}

//  If message contains value "endgame" then we know winner
//  and loser, and we display messages on the scene for each
//  player. When it's done we write it to the socket.
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


//  If message contains value quit and the game it's not over
//  then we display message for player who won.
void Main::messageQuit()
{
    if (m_obj.value("quit") != m_name) {
        gameOverDesign("YOU WON!");
    }
}
