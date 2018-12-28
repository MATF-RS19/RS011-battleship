#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include "ship.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "square.h"
#include "Player.h"

namespace Ui {
class Main;
}

class Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main(QWidget *parent = 0);
    void pickedSquare(Square* square);
    void moveShip(const QPointF startPoint, int length, bool isVertical) const;
    void start();
    ~Main();
     QGraphicsScene *scene;
     Square *pSquare = nullptr;
     Square *kSquare = nullptr;
private slots:
    void on_Ready_clicked();

private:
    Ui::Main *ui;
    Board *board1;
    Board *board2;
    Ship *brod1;
    Ship *brod2;
    Ship *brod3;
    Ship *brod4;
    Player *m_p;
    bool m_lock = 0;
};

#endif // MAINWINDOW_H
