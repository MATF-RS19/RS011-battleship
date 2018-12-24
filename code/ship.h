#ifndef SHIP_H
#define SHIP_H

#include <QString>
#include <QGraphicsItem>

class Ship : public QGraphicsItem
{
public:
    Ship(QString name, int length);

    QRectF boundingRect() const override;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *widget) override;

private:
    int m_length;
    QString m_name;
};

#endif // SHIP_H
