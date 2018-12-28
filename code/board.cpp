#include "board.h"

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QtWidgets>
#include <QDebug>
#include <QGraphicsScene>
#include <algorithm>

Board::Board()
{

}

QList<Square *> Board::getSquares()
{
    return squares;
}

void Board::placeSquares(int x,int y,int cols,int rows, bool r, QGraphicsScene *scene)
{
    for (int i = 0; i < cols; i++){
        createSquareColumn(x + i*20, y, rows, r, i, scene);
    }
}

void Board::clickedOn(int x1, int y1, int x2, int y2)
{
    qDebug() << "i dalje";
    if ((x1 == x2) & (y1 != y2)) {
        int length = std::abs(y1 - y2);
        if (y1 > y2) {
            while(length >= 0) {
                findSquare(x1, y1-length);
                length--;
            }
        } else {
            while(length >= 0) {
                findSquare(x1, y1+length);
                length--;
            }
        }
    } else if ((y1 == y2) & (x1 != x2)) {
        int length = std::abs(x1 - x2);
        if (x1 > x2) {
            while (length >= 0) {
                findSquare(x1 - length, y1);
                length--;
            }
        } else {
            while (length >= 0) {
                findSquare(x1 + length, y1);
                length--;
            }
        }
    }
     qDebug() << "i dalje";
}

void Board::findSquare(int i, int j)
{
    for (auto& s : squares) {
        if ((s->getI() == i) & (s->getJ() == j)) {
            s->setSelected(true);
        }
    }
}

void Board::createSquareColumn(int x, int y, int rows, bool r, int curr_i, QGraphicsScene *scene)
{
    Q_UNUSED(r);
    for(int i = 0; i < rows; i++) {
        Square* square = new Square();
        square->setPos(x, y+i*20);
        square->setPlaced(true);
        square->setI(i);
        square->setJ(curr_i);
        square->setPlacement(i, curr_i);
        squares.append(square);
        scene->addItem(square);
    }
}
