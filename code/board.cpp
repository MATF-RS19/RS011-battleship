#include "board.h"
#include "ui_board.h"

#include <QPaintEvent>
#include <QPainter>

Board::Board(QWidget *parent) :
    QWidget(parent)
{

}

Board::~Board()
{
    delete ui;
}

void Board::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);
    QPainter p(this);
    p.drawRect(10, 0, 200, 200);
    p.drawRect(290, 0, 200, 200);
    for (int i = 1; i < 10; i++) {
        p.drawLine(10+i*20, 0, 10+i*20, 200);
        p.drawLine(10, i*20, 210, i*20);

        p.drawLine(290+i*20, 0, 290+i*20, 200);
        p.drawLine(290, i*20, 490, i*20);
    }
}
