#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include "ship.h"
#include "square.h"
#include "player.h"
#include "machine.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QJsonObject>
#include <QTcpSocket>
#include <QHostAddress>

namespace Ui {
class Main;
}

class Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main(QWidget *parent = 0);
    ~Main();

    bool connectToGameServer(QString name, QHostAddress address, int port);
    void singlePlayer(QString naame, QString opponent);

    void drawScene();
    void drawBoards();
    void drawShips();

    void game();
    void machinesAttack();

    void moveShip(const QPointF startPoint, int length, bool isVertical, std::vector<int> coords) const;
    void pickedSquare(Square* square);
    void displayClickOnSquare(bool correctGuess, QPointF position);
    void gameOverDesign(QString text);

    void deinit();

    void connectSquaresToPlayersAttackFunction();
    void disconnectSquaresToPlayersAttackFunction();
    void enableMove();
    void disableMove();

    bool getLock() const;

    QGraphicsScene *scene;
    Square *startSquare = nullptr;
    Square *endSquare = nullptr;

    Board *m_board1;
    Board *m_board2;

private slots:
    void onReadyClicked();
    void restart();
    void playersAttack(int i, int j, QPointF position);
    void playMove(int i, int j, QPointF position);
    void processMessage();
    void closeForTwoPlayers();

private:
    Ui::Main *ui;
    Player *m_player;
    Machine *m_machine;
    QString m_name;
    QString m_opponent;
    QTcpSocket *m_socket;
    QJsonObject m_obj;
    QTcpSocket *socket;

    bool m_lock = 0;
    int m_mode;
    int m_countSinked = 0;
    bool m_clicked = false;
    QPointF m_posAttack;
    bool m_finished;

    int checkBoard(int i, int j, Board *board);
    void findPositionOfOpponentsAttack(int i, int j);

    void messageStartGame();
    void messageTurn();
    void messageCheck();
    void messageDisplay();
    void messageEndGame();
    void messageQuit();
};

#endif // MAINWINDOW_H
