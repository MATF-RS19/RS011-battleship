#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <QPaintEvent>

class Player
{
public:
    Player(QString name);
private:
    QString m_name;
    QString m_messages;
   // std::vector<Unit> ships;
};

#endif // PLAYER_H
