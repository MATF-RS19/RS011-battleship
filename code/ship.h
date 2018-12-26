#ifndef SHIP_H
#define SHIP_H

#include <QString>
#include <QGraphicsItem>

class Ship : public QGraphicsItem
{
public:
    Ship(QString name, int length);

    QRectF boundingRect() const;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
protected:
    void setAcnhorPoint(const QPointF &anchorPoint);
private:
    QString m_name;
    int m_length;
    QPointF anchorPoint;
    bool m_draged;
};

#endif // SHIP_H
