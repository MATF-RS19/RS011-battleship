#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QWidget>

class Square : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:

    Square(QGraphicsItem* parent = NULL);
    bool getPlaced() const;
    bool getFirstBoard()const;
    int getI() const;
    int getJ() const;
    bool getAttacked() const;
    bool getSelected() const;

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    void setPlaced(bool b);
    void setFirstBoard(bool b);
    void setI(int i);
    void setJ(int j);
    void setAttacked();
    void setSelected(bool b);

    int x1, x2, y1, y2;
    int count = 2;

    void checkIfSquareIsOnBorder();
    bool squareIsOnBorder();
    bool checkIfSquareIs(QString border);

signals:
    void squareClicked(int i, int j, QPointF position);
private:
    bool m_placed;
    bool m_FirstBoard;
    int m_i;
    int m_j;
    bool m_attacked;
    bool m_selected;

    QMap<QString, bool> m_borderSquares;
};

#endif // SQUARE_H
