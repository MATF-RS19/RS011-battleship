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
    bool getSelected();

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    void setPlaced(bool b);
    void setSecond(bool b);
    void setI(int i);
    void setJ(int j);
    void setSelected(bool b);

private:
    bool m_placed;
    bool m_second;
    int m_i;
    int m_j;
    bool m_selected;
};

#endif // SQUARE_H
