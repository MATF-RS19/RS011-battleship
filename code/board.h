#ifndef BOARD_H
#define BOARD_H

#include <QFrame>

namespace Ui {
class Board;
}

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = 0);
    ~Board();

public slots:
    void paintEvent(QPaintEvent *e);

private:
    Ui::Board *ui;
};

#endif // BOARD_H
