#ifndef SHIP_H
#define SHIP_H

#include <QString>
#include <QGraphicsItem>
#include <QGraphicsRectItem>

class Ship : public QGraphicsRectItem
{
public:
    Ship(int length);

    QRectF boundingRect() const override;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget) override;
    void setAnchorPoint(const QPointF &anchorPoint);

    int getShipLength() const;
    void setIsVertical(bool val);
    bool getIsVertical() const;
    void setPlacedShip();
    bool getPlacedShip() const;
    void rotationShip();
    std::vector<int> getCoordinates() const;
    void setCoordinates(std::vector<int> coords);

private:
    int m_length;
    QPointF m_anchorPoint;
    bool m_placed;
    bool m_isVertical = 1;
    std::vector<int> m_coordinates;
};

#endif // SHIP_H
