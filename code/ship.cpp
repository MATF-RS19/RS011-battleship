#include "ship.h"
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QString>

#include <QGraphicsSceneDragDropEvent>
#include <QtWidgets>
#include <QGraphicsItem>
#include <QDebug>

Ship::Ship(QString name, int length) :
    m_name(name), m_length(length), m_draged(false)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF Ship::boundingRect() const
{
    return QRectF(290 + 20 * (m_length - 2), 100, 13, m_length * 20);
}

void Ship::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);
    QPixmap source(":/ship_texture.jpg");
    int h = source.height();
    int w = source.width();
    source = source.scaled(w/30, h/30);
    painter->setBrush(source);
    painter->drawEllipse(290 + 20 * (m_length - 2), 100, 13, m_length * 20);

}

void Ship::setAcnhorPoint(const QPointF &anchorPoint) {
    this->anchorPoint = anchorPoint;
}

void Ship::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    QGraphicsItem::mousePressEvent(event);
}

void Ship::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_draged = true;
    QGraphicsItem::mouseMoveEvent(event);
}

void Ship::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    qDebug() << event->screenPos();
    QGraphicsItem::mouseReleaseEvent(event);

}

