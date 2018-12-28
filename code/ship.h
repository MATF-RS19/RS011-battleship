#ifndef SHIP_H
#define SHIP_H

#include <QString>
#include <QGraphicsItem>
#include <QGraphicsRectItem>

class Ship : public QGraphicsRectItem
{
public:
    Ship(QString name, int length);

    QRectF boundingRect() const override;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void setAnchorPoint(const QPointF &anchorPoint);

    int getShipLength() const;
    void setIsVertical(bool val);
    bool getIsVertical();
    void rotationShip();

private:
    QString m_name;
    int m_length;
    QPointF anchorPoint;
    bool m_draged;
    bool isVertical = 1;
};

#endif // SHIP_H
