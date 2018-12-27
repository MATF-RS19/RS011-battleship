#include "square.h"
#include <QDebug>

Square::Square(QGraphicsItem *parent){
    Q_UNUSED(parent);

    QRect sqPoints = QRect(QPoint(0,0),QPoint(20,20));

    QRectF square(sqPoints);

    setRect(square);

    m_placed = false;
    m_second = false;
    m_selected = false;
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

bool Square::getSelected()
{
    return m_selected;
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

void Square::setI(int i) {
    m_i = i;
}

void Square::setJ(int j) {
    m_j = j;
}

void Square::setSelected(bool b)
{
    m_selected = b;
}
