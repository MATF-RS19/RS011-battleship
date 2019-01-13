#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include <QList>
#include <QGraphicsScene>

class Board {

public:
    Board();

    QList<Square*> getSquares() const;
    void placeSquares(int x, int y, int rows, int columns, bool isFirst, QGraphicsScene *scene);

    bool selectedSquares(int x1, int y1, int x2, int y2);
    bool findSquare(int i, int j);
    void restartSelected();

    void setNamePlayer(QString name);
private:
    QString m_namePlayer;
    QList<Square*> m_squares;
};

#endif // BOARD_H
