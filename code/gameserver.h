#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "mainwindow.h"

#include <QDialog>
#include <QTcpServer>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class GameServer;
}

class GameServer : public QDialog
{
    Q_OBJECT

public:
    explicit GameServer(QWidget *parent = 0);
    ~GameServer();

    Main *m_player;

    int getMode() const;

    void keyPressEvent(QKeyEvent *e);

private slots:
    void onSinglePlayer();
    void onTwoPlayers();
    void onClose();
    void enable();

private:
    Ui::GameServer *ui;

    QString m_addr;
    QString m_name;
    int m_move = 1;
    int m_count = 0;
    int m_mode;
};

#endif // GAMESERVER_H
