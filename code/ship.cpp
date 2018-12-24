#include "ship.h"
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QString>

Ship::Ship(QString name, int length) :
    m_name(name), m_length(length)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

QRectF Ship::boundingRect() const
{
    return QRect(15 * m_length, 15 * m_length, 15 * m_length,15 * m_length);
}

void Ship::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QPixmap source(":/ship_texture.jpg");
    int h = source.height();
    int w = source.width();
    source = source.scaled(w/30, h/30);
    painter->setBrush(source);
    painter->drawEllipse(20 * m_length, 20, 13, m_length * 18);

}
