#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include "ship.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "square.h"
#include "player.h"
#include "machine.h"

namespace Ui {
class Main;
}

class Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main(QWidget *parent = 0);
    void pickedSquare(Square* square);
    void moveShip(const QPointF startPoint, int length, bool isVertical, std::vector<int> coords) const;
    void init();
    void deinit();
    bool getLock() const;

    void game();
    void gameOverDesign(QString text);
    void displayClickOnSquare(int x, int y, bool correctGuess, QString whoIsPlaying);
    void connectSquaresToPlayersAttackFunction();
    void disconnectSquaresToPlayersAttackFunction();
    void machinesAttack();

    ~Main();
     QGraphicsScene *scene;
     Square *startSquare = nullptr;
     Square *endSquare = nullptr;

private slots:
    void on_Ready_clicked();
    void restart();
    void playersAttack(int i, int j, QPointF position);

private:
    Ui::Main *ui;
    Board *m_board1;
    Board *m_board2;
    Player *m_player;
    Machine *m_machine;
    bool m_lock = 0;
};

#endif // MAINWINDOW_H
