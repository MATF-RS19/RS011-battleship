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

Ship::Ship(QString name, int length) :
    m_name(name), m_length(length), m_draged(false)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF Ship::boundingRect() const
{
    return QRectF(240+20*m_length, 100, 13, m_length*20);
}

void Ship::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    Q_UNUSED(option);
    QPixmap source(":/ship_texture.jpg");
    int h = source.height();
    int w = source.width();
    source = source.scaled(w/30, h/30);
    painter->setBrush(source);
    painter->drawEllipse(240+20*m_length, 100, 13, m_length * 20);
    // painter->translate(237, 80);

}

void Ship::setAnchorPoint(const QPointF &anchorPoint) {
    this->anchorPoint = anchorPoint;
}
int Ship::getShipLength() const
{
    return m_length;
}

void Ship::setIsVertical(bool val)
{
    isVertical = val;
}

bool Ship::getIsVertical()
{
    return isVertical;
}

void Ship::rotationShip()
{

    //this->setRotation(90);
    QPointF offset = this->boundingRect().center();
    QTransform transformation;
    transformation.translate(offset.x(), offset.y());
    transformation.rotate(90);
    transformation.translate(-offset.x(), -offset.y());
    this->setTransform(transformation);
    /*
        QPointF offset = gr->sceneBoundingRect().center();

        QTransform transform;
        transform.translate(offset.x(),offset.y());
        transform.rotate(90);
        transform.translate(-offset.x(),-offset.y());
        gr->setTransform(transform);

        scene->destroyItemGroup(gr);
        scene->update();*/
}
void Ship::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ClosedHandCursor);
    QGraphicsRectItem::mousePressEvent(event);
}

void Ship::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_draged = true;
    QGraphicsRectItem::mouseMoveEvent(event);
}

void Ship::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::OpenHandCursor);
    qDebug() << this->pos();
    QGraphicsRectItem::mouseReleaseEvent(event);

}

