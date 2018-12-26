#include "board.h"
#include "ui_board.h"

#include <QGraphicsSceneDragDropEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QtWidgets>
#include <QDebug>

Board::Board(int x, int y)
    : m_x(x), m_y(y), m_drag_over(false) {
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptDrops(true);
}

QRectF Board::boundingRect() const {
    return QRect(0, 0, 200, 200);
}

void Board::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
    Q_UNUSED(option);
    painter->drawRect(m_x, m_y, 200, 200);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            painter->drawRect(m_x+i*20, m_y+j*20,20, 20);
        }
    }
}

void Board::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
//    event->setAccepted(true);
    m_drag_over = true;
    qDebug() << "##" << event->pos();
    QGraphicsItem::dragLeaveEvent(event);
}

void Board::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    m_drag_over = false;
    qDebug() << "##" << event->pos();
    QGraphicsItem::dragLeaveEvent(event);
}

void Board::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    m_drag_over = false;
    qDebug() << "##" << event->pos();
    QGraphicsItem::dragLeaveEvent(event);
}

void Board::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "@@@@@" << event->screenPos();

}

//void Board::paintEvent(QPaintEvent *e) {
//    QWidget::paintEvent(e);
//    QPainter p(this);
//    p.drawRect(10, 0, 200, 200);
//    p.drawRect(290, 0, 200, 200);
//    for (int i = 1; i < 10; i++) {
//        p.drawLine(10+i*20, 0, 10+i*20, 200);
//        p.drawLine(10, i*20, 210, i*20);

//        p.drawLine(290+i*20, 0, 290+i*20, 200);
//        p.drawLine(290, i*20, 490, i*20);
//    }
//}

