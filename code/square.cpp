#include "board.h"
#include "square.h"
#include "mainwindow.h"
#include "gameserver.h"

#include <QDebug>

extern GameServer* w;

Square::Square(QGraphicsItem *parent){
    Q_UNUSED(parent);

    QRect sqPoints = QRect(QPoint(0,0),QPoint(20,20));

    QRectF square(sqPoints);

    setRect(square);

    m_placed = false;
    m_FirstBoard = false;
    m_selected = false;
    m_attacked = false;

    m_borderSquares["topBorder"] = false;
    m_borderSquares["leftBorder"] = false;
    m_borderSquares["rightBorder"] = false;
    m_borderSquares["bottomBorder"] = false;

    m_borderSquares["topLeftCorner"] = false;
    m_borderSquares["topRightCorner"] = false;
    m_borderSquares["bottomLeftCorner"] = false;
    m_borderSquares["bottomRightCorner"] = false;
}

bool Square::getPlaced() const
{
    return m_placed;
}

bool Square::getFirstBoard() const
{
    return m_FirstBoard;
}

int Square::getI() const
{
    return m_i;
}

int Square::getJ() const
{
    return m_j;
}

bool Square::getAttacked() const
{
    return m_attacked;
}

bool Square::getSelected() const
{
    return m_selected;
}

void Square::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if (w->getMode() == 2) {
        if ( this->getFirstBoard()){//when clicked on player's board, for ship setting
            w->m_player->pickedSquare(this);
        } else if( !this->getFirstBoard() && !this->getAttacked()){//when clicked on square that was not attacked, on opponent's board, for attack
            this->setAttacked();
            emit squareClicked(this->getI(), this->getJ(), this->pos());
        }
    } else {
        if(this->getFirstBoard()){//when clicked on player's board, for ship setting
            w->m_player->pickedSquare(this);
        }//when clicked on square that was not attacked, on opponent's board and all ships are positioned, for attack
        else if (!this->getFirstBoard() && w->m_player->getLock() && !this->getAttacked()){
            this->setAttacked();
            emit squareClicked(this->getI(), this->getJ(), this->pos());
        }
    }

}

void Square::setPlaced(bool b)
{
    m_placed = b;
}

void Square::setFirstBoard(bool b)
{
    m_FirstBoard = b;
}

void Square::setI(int i) {
    m_i = i;
}

void Square::setJ(int j) {
    m_j = j;
}

void Square::setAttacked()
{
    m_attacked = true;
}

void Square::setSelected(bool b)
{
    m_selected = b;
}
void Square::checkIfSquareIsOnBorder() {
    if (m_i == 0 && m_j == 0) {
        m_borderSquares["topLeftCorner"] = true;
    } else if (m_i == 0 && m_j == 9) {
        m_borderSquares["topRightCorner"] = true;
    } else if (m_i == 0) {
        m_borderSquares["topBorder"] = true;
    } else if (m_i == 9 && m_j == 0) {
        m_borderSquares["bottomLeftCorner"] = true;
    } else if (m_i == 9 && m_j == 9) {
        m_borderSquares["bottomRightCorner"] = true;
    } else if (m_i == 9) {
        m_borderSquares["bottomBorder"] = true;
    } else if (m_j == 0) {
        m_borderSquares["leftBorder"] = true;
    } else if (m_j == 9) {
        m_borderSquares["rightBorder"] = true;
    }
}

bool Square::squareIsOnBorder()
{
    if (m_borderSquares["topLeftCorner"]) {
        return true;
    } else if (m_borderSquares["topRightCorner"]) {
        return true;
    } else if (m_borderSquares["bottomLeftCorner"]) {
        return true;
    } else if (m_borderSquares["bottomRightCorner"]) {
        return true;
    } else if (m_borderSquares["topBorder"]) {
        return true;
    } else if (m_borderSquares["bottomBorder"]) {
        return true;
    } else if (m_borderSquares["leftBorder"]) {
        return true;
    } else if (m_borderSquares["rightBorder"]) {
        return true;
    } else {
        return false;
    }


}

bool Square::checkIfSquareIs(QString border)
{
    return m_borderSquares[border];
}
