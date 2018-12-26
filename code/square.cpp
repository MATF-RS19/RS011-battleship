#include "square.h"
#include <QDebug>

Square::Square(QGraphicsItem *parent){
    Q_UNUSED(parent);

    QRect sqPoints = QRect(QPoint(0,0),QPoint(20,20));

    QRectF square(sqPoints);

    setRect(square);

    m_placed = false;
    m_second = false;
}

bool Square::getPlaced()
{
    return m_placed;
}

bool Square::getSecond()
{
    return m_second;
}

int Square::getI()
{
    return m_i;
}

int Square::getJ()
{
    return m_j;
}

void Square::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    qDebug() << QString("mousePressEvent") << this->pos() << this->getI() << this->getJ();
}

void Square::setPlaced(bool b)
{
    m_placed = b;
}

void Square::setSecond(bool b)
{
    m_second = b;
}

void Square::setPlacement(int i, int j)
{
    m_i = i;
    m_j = j;
}
