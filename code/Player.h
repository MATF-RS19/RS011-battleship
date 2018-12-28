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
private:
    QString m_name;
    QString m_messages;
   std::vector<Ship*> m_ships;
};

#endif // PLAYER_H
