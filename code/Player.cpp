#include "Player.h"
#include <QString>
#include <QDebug>

Player::Player(QString name)
    :m_name(name){
}

void Player::init_ships()
{

    Ship *brod1 = new Ship(QString("Submarine"), 2);
    brod1->setFlag(QGraphicsItem::ItemIsSelectable, true);
    brod1->setFlag(QGraphicsItem::ItemIsMovable, true);

    m_ships.push_back(brod1);

    Ship *brod2 = new Ship(QString("Submarine"), 3);
    brod2->setFlag(QGraphicsItem::ItemIsSelectable, true);
    brod2->setFlag(QGraphicsItem::ItemIsMovable, true);


    m_ships.push_back(brod2);

    Ship *brod3 = new Ship(QString("Submarine"), 4);
    brod3->setFlag(QGraphicsItem::ItemIsSelectable, true);
    brod3->setFlag(QGraphicsItem::ItemIsMovable, true);


    m_ships.push_back(brod3);

    Ship *brod4 = new Ship(QString("Submarine"), 5);
    brod4->setFlag(QGraphicsItem::ItemIsSelectable, true);
    brod4->setFlag(QGraphicsItem::ItemIsMovable, true);


    m_ships.push_back(brod4);

}

std::vector<Ship*> Player::getShips() const
{
    return m_ships;
}

