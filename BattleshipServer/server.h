#ifndef SERVER_H
#define SERVER_H

#include <QDialog>

#include <QJsonObject>
#include <QLabel>
#include <QNetworkSession>
#include <QTcpServer>
#include <vector>

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

    void startServer();

    void keyPressEvent(QKeyEvent *e);

private slots:
    void newConnection();
    void handle();

    void read(QTcpSocket *sock);
    void turn();
    void hit();
    void check(QTcpSocket *sock);
    void end();
    void done();
    void quit(QTcpSocket *sock);

private:
    Ui::Server *ui;

    QTcpServer *m_server;           //  server
    QHostAddress m_addr;            //  IP address
    int m_port = 4242;              //  default port

    QJsonObject m_obj;              //  object for writing messages to players
    QString m_name1, m_name2;       //  variables for names of players

    QTcpSocket *sock1;              //  QTcpSocket for first connected player
    qintptr sd1 = 0;                //  socket descriptor for first player
    QTcpSocket *sock2;              //  QTcpSocket for second connected player
    qintptr sd2 = 0;                //  socket descriptor for second player

    int m_count = 0;                //  counter of how many players are in the game
    int m_ready = 0;                //  counter of how many players are ready
    int m_quit = 2;                 //  counter of how many players have exited the game
};

#endif // SERVER_H
