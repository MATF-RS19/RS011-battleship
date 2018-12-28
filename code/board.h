#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include <QList>
#include <QGraphicsScene>

class Board {

public:
    Board();

    QList<Square*> getSquares();
    void placeSquares(int x, int y, int cols, int rows, bool r, QGraphicsScene *scene);

    void clickedOn(int x1, int y1, int x2, int y2);
    void findSquare(int i, int j);
private:
    QList<Square*> squares;
    void createSquareColumn(int x, int y, int rows, bool r, int curr_i, QGraphicsScene *scene);























    /*
public:
    Board(int x, int y);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


private:
    int m_x;
    int m_y;
    bool m_drag_over;*/
};

#endif // BOARD_H
