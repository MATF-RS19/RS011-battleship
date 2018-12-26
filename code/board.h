#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsItem>

class Board : public QGraphicsItem {
public:
    Board(int x, int y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    int m_x;
    int m_y;
    bool m_drag_over;
};

#endif // BOARD_H
