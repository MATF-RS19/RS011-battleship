#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "board.h"
#include "ship.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

namespace Ui {
class Main;
}

class Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main(QWidget *parent = 0);
    ~Main();

private:
    Ui::Main *ui;
    Board *board1;
    Board *board2;
    Ship *brod1;
    Ship *brod2;
    Ship *brod3;
    Ship *brod4;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
