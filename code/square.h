#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Square : public QGraphicsRectItem
{
public:

    Square(QGraphicsItem* parent = NULL);
    bool getPlaced();
    bool getSecond();
    int getI();
    int getJ();
    bool getSelected();

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    void setPlaced(bool b);
    void setSecond(bool b);
    void setPlacement(int i, int j);
    void setI(int i);
    void setJ(int j);
    void setSelected(bool b);

    int x1, x2, y1, y2;
    int count = 2;
private:
    bool m_placed;
    bool m_second;
    int m_i;
    int m_j;
    bool m_selected;
};

#endif // SQUARE_H
