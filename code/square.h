#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Square : public QGraphicsRectItem
{
public:

    Square(QGraphicsItem* parent=NULL);
    bool getPlaced();
    bool getSecond();
    int getI();
    int getJ();

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    void setPlaced(bool b);
    void setSecond(bool b);
    void setPlacement(int i, int j);

private:
    bool m_placed;
    bool m_second;
    int m_i;
    int m_j;

};

#endif // SQUARE_H
