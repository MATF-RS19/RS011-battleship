#include "ship.h"
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QString>

#include <QGraphicsSceneDragDropEvent>
#include <QtWidgets>
#include <QGraphicsItem>
#include <QDebug>
#include "board.h"
#define START_POSITION_X 240
#define START_POSITION_Y 100
#define UNIT_LENGTH 20
#define SHIP_WIDTH 13

Ship::Ship(int length) :
    m_length(length), m_placed(false)
{

}

QRectF Ship::boundingRect() const//positioning of the ship
{
    return QRectF(START_POSITION_X+UNIT_LENGTH*m_length, START_POSITION_Y, SHIP_WIDTH, m_length*UNIT_LENGTH);
}

void Ship::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);
    QPixmap source(":/ship_texture.jpg");
    int h = source.height();
    int w = source.width();
    source = source.scaled(w/30, h/30);
    painter->setBrush(source);
    painter->drawEllipse(START_POSITION_X+UNIT_LENGTH*m_length, START_POSITION_Y, SHIP_WIDTH, m_length * UNIT_LENGTH);
}

void Ship::setAnchorPoint(const QPointF &anchorPoint) {
    this->m_anchorPoint = anchorPoint;
}
int Ship::getShipLength() const
{
    return m_length;
}

void Ship::setIsVertical(bool val)
{
    m_isVertical = val;
}

bool Ship::getIsVertical() const
{
    return m_isVertical;
}
void Ship::setPlacedShip()
{
    m_placed = true;
}

bool Ship::getPlacedShip() const
{
    return m_placed;
}
void Ship::rotationShip()//rotation of the ship by 90 degrees
{
    QPointF offset = this->boundingRect().center();
    QTransform transformation = this->transform();
    transformation.translate(offset.x(), offset.y());
    transformation.rotate(90);
    transformation.translate(-offset.x(), -offset.y());
    this->setTransform(transformation);
}

std::vector<int> Ship::getCoordinates() const
{
    return m_coordinates;
}
void Ship::setCoordinates(std::vector<int> coords)
{
    m_coordinates = coords;
}
