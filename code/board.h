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
};

#endif // BOARD_H
