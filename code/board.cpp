#include "board.h"

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QtWidgets>
#include <QDebug>
#include <QGraphicsScene>
#include <algorithm>
#include <iostream>

Board::Board()
{

}

QList<Square *> Board::getSquares() const
{
    return m_squares;
}

void Board::placeSquares(int x, int y, int rows, int columns, bool isFirst, QGraphicsScene *scene)
{
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++) {
            Square* square = new Square();
            square->setPos(x+i*20, y+j*20);
            square->setPlaced(true);
            square->setFirstBoard(isFirst);
            square->setI(j);
            square->setJ(i);
            square->checkIfSquareIsOnBorder();
            m_squares.append(square);
            scene->addItem(square);
        }
    }
}
//the function selects squares between given positions
bool Board::selectedSquares(int x1, int y1, int x2, int y2)
{
    if ((x1 == x2) & (y1 != y2)) {
        int length = std::abs(y1 - y2);
        if (y1 > y2) {
            while(length >= 0) {
                if(!findSquare(x1, y1-length))
                    return false;
                length--;
            }
        } else {
            while(length >= 0) {
                if(!findSquare(x1, y1+length))
                    return false;
                length--;
            }
        }
    } else if ((y1 == y2) & (x1 != x2)) {
        int length = std::abs(x1 - x2);
        if (x1 > x2) {
            while (length >= 0) {
                if(!findSquare(x1 - length, y1))
                    return false;
                length--;
            }
        } else {
            while (length >= 0) {
                if(!findSquare(x1 + length, y1))
                    return false;
                length--;
            }
        }
    }
    return true;
}
//the function finds squares and selects them
bool Board::findSquare(int i, int j)
{
    for (auto& s : m_squares) {
        if ((s->getI() == i) & (s->getJ() == j)) {
            if (s->getSelected()){
                return false;
            }
            s->setSelected(true);
        }
    }
    return true;
}
//the function sets flag selected for all squares to false
void Board::restartSelected()
{
    for(auto s : m_squares){
        s->setSelected(false);
    }
}

void Board::setNamePlayer(QString name)
{
    m_namePlayer = name;
}
