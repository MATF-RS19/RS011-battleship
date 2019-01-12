#include "player.h"
#include <QString>
#include <QDebug>

Player::Player(QString name)
    :m_name(name){
}
Player::~Player(){
    for(auto ship : m_ships){
        delete ship;
    }
}
void Player::init_ships()//ship length goes from 2 to 5
{
    Ship *brod;
    for(int i = 2; i<6;i++){
        brod = new Ship(i);
        m_ships.push_back(brod);
    }
}

std::vector<Ship*> Player::getShips() const
{
    return m_ships;
}

int Player::getCorrectGuesses() const
{
    return m_countCorrectGuessesPlayer;
}

void Player::setCorrectGuesses(int number)
{
    m_countCorrectGuessesPlayer = number;
}

bool Player::IWon()//ship length goes from 2 to 5, the player won if all ships sunk
{
    if (m_countCorrectGuessesPlayer == (2 + 3 + 4 + 5))
        return true;

    return false;
}

