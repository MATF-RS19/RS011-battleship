#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <QPaintEvent>
#include "ship.h"

class Player
{
public:
    Player(QString name);
    void init_ships();
    std::vector<Ship*> getShips() const;
    int getCorrectGuesses() const;
    void setCorrectGuesses(int number);
    bool IWon();
    ~Player();
private:
    QString m_name;
    std::vector<Ship*> m_ships;
    int m_countCorrectGuessesPlayer = 0;
};

#endif // PLAYER_H
