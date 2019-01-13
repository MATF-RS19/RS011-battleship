#include "server.h"
#include "ui_server.h"

#include <QDebug>
#include <QIODevice>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QTcpSocket>
#include <string>

//  Setting background of the dialog, starting server
//  and connecting buttons.
Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    this->setWindowTitle("BattleshipServer");

    QPixmap pix(":/radar.jpg");
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, pix);
    this->setPalette(palette);

    ui->label->setAlignment(Qt::AlignHCenter);
    ui->label->setStyleSheet("QLabel {color: white}");

    startServer();
    connect(ui->button, SIGNAL(clicked(bool)), this, SLOT(close()));
}

Server::~Server()
{
    delete ui;
}

//  Creating instance of QTcpServer and chosing IP address on which
//  m_server will listen for new connections. Default port is 4242.
//  Depending on result of listen() appropriate message is shown.
void Server::startServer()
{
    m_server = new QTcpServer(this);
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (auto addr : list) {
        if (addr != QHostAddress::LocalHost && addr.toIPv4Address()) {
            m_addr = addr;
            break;
        }
    }
    if (m_addr.toString().isEmpty())
        m_addr = QHostAddress::LocalHost;

    if (!m_server->listen(m_addr, m_port)) {
        qDebug() << "Server could not start";
        ui->label->setText("Server could not start");
    } else {
        qDebug() << "Server started on " + m_addr.toString() + " on port " << m_port;
        ui->label->setText("Server started");
        ui->lineEdit->setText(m_addr.toString());
        ui->lineEdit->setAlignment(Qt::AlignHCenter);
    }
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

//  Redefining function keyPressEvent because we have to forbid dialog
//  to close when ESC button is clicked.
void Server::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_Escape) {
        QDialog::keyPressEvent(e);
    } else {
        e->ignore();
    }
}

//  When server has pending connection we create socket and connect
//  it to SIGNAL(readyRead()) and SLOT(handle()) which means that
//  whenever socket has something to read function handle() will be
//  called to process the message.
void Server::newConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket *sock = m_server->nextPendingConnection();
        connect(sock, SIGNAL(readyRead()), this, SLOT(handle()));
    }
}

//  This function processes each message server receives and depending on the
//  message appropriate function is called.
void Server::handle()
{
    //  Creating QTcpSocket based on the sender.
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(sender());

    //  Because we allow only two players to play the game we have to set up
    //  variables for each one of the players so we can distinguish players
    //  based on their socket descriptors.
    if (sd1 == 0) {
        sd1 = sock->socketDescriptor();
        sock1 = new QTcpSocket(sock);
        sock1->setSocketDescriptor(sd1, QAbstractSocket::ConnectedState,
                                   QIODevice::ReadWrite);
    } else if (sd2 == 0 && sd1 != 0 && sd1 != sock->socketDescriptor()) {
        sd2 = sock->socketDescriptor();
        sock2 = new QTcpSocket(sock);
        sock2->setSocketDescriptor(sd2, QAbstractSocket::ConnectedState,
                                   QIODevice::ReadWrite);
    }

    //  For communication between server and clients we are using QJsonDocuments
    //  and QJsonObjects.
    QJsonDocument msg = QJsonDocument::fromJson(sock->readAll());
    m_obj = msg.object();
    qDebug() << "Server handle: " << m_obj;

    //  When counter of ready players is equal to two then new value "startgame"
    //  is added to QJsonObject and we write it to both sockets of players. Old
    //  values that we dont need anymore are removed from the object.
    if (m_ready == 2) {
        qDebug() << "ready two" << m_obj;
        m_obj.remove("wait");
        m_obj.remove("name");
        m_obj.insert("startgame", m_name1);
        m_obj.insert("opponent", m_name2);
        QJsonDocument o(m_obj);
        sock1->write(o.toJson());
        sock2->write(o.toJson());
        m_ready = 0;
        return;
    }

    if (m_obj.contains("ready")) {
        read(sock);
        return;
    }
    if (m_obj.contains("hit")) {
        hit();
        return;
    }
    if (m_obj.contains("i") && m_obj.contains("j") && m_obj.contains("check")) {
        check(sock);
        return;
    }

    if (m_obj.contains("turn") && !m_obj.contains("sinked")) {
        turn();
        return;
    }

    if (m_obj.contains("winner")) {
        end();
        return;
    }

    if (m_obj.contains("done")) {
        done();
        return;
    }

    if (m_obj.contains("quit")) {
        quit(sock);
        return;
    }
}

//  If QJsonObject contains value "ready" then we have to set up names
//  for the players because we will need that variables in later
//  communication. When two players are ready and their names are set
//  we add new message to status label in dialog.
void Server::read(QTcpSocket *sock)
{
    qDebug() << "read" << m_obj;

    if (m_count < 2)
        m_count++;
    if (m_count == 1) {
        m_name1 = m_obj.value("name").toString();
    } else if (m_count == 2) {
        if (m_obj.value("name").toString() != m_name1)
            m_name2 = m_obj.value("name").toString();
        ui->status->setText("There is no spots left for new players");
        ui->status->setStyleSheet("QLabel {color:white}");
    }

    //  Because QJsonObject contains value ready then we have to
    //  increment value of m_ready for each player. We remove value
    //  "ready", insert new value "wait" and write it to the socket.
    if (m_obj.value("ready").toString() == m_name1) {
        m_ready++;
        m_obj.remove("ready");
        m_obj.insert("wait", "other");
        QJsonDocument o(m_obj);
        sock->write(o.toJson());
    } else if (m_obj.value("ready").toString() == m_name2) {
        m_ready++;
        m_obj.remove("ready");
        m_obj.insert("wait", "other");
        QJsonDocument o(m_obj);
        sock->write(o.toJson());
    }

    qDebug() << "end read" << m_obj;
}

//  If QJsonObject we received contains value "turn" then we write it
//  to both sockets of our players.
void Server::turn()
{
    qDebug() << "turn " << m_obj;
    QJsonDocument o(m_obj);
    sock1->write(o.toJson());
    sock2->write(o.toJson());
}

//  If QJsonObject contains value "hit" then we have to insert new
//  values so that players could display attacks on their boards.
void Server::hit()
{
    qDebug() << "hit server" << m_obj;
    if (m_obj.contains("sinked")) {
        m_obj.insert("winner", m_obj.value("ph"));
        m_obj.insert("loser", m_obj.value("next"));
    }

    m_obj.insert("display", m_obj.value("ph"));
    m_obj.insert("value", m_obj.value("hit").toBool());
    m_obj.remove("check");
    m_obj.remove("hit");
    m_obj.remove("ph");
    m_obj.remove("turn");
    QJsonDocument o(m_obj);
    sock1->write(o.toJson());
    sock2->write(o.toJson());
}

//  If QJsonObject contains value "check" we insert new values
//  and write changed QJsonObject to the opposite socket of our
//  receiving socket.
void Server::check(QTcpSocket *sock)
{
    qDebug() << "server check" << m_obj;
    m_obj.insert("now", m_obj.value("turn").toString());
    m_obj.insert("next", m_obj.value("opponent").toString());
    QJsonDocument o(m_obj);
    if (sock->socketDescriptor() == sd1)
        sock2->write(o.toJson());
    else
        sock1->write(o.toJson());
}

//  If QJsonObject contains value "winner" then we remove all
//  values that we don't need anymore and write it to sockets
//  of both players. Because our game is over we set socket
//  descriptors to 0 so server could receive new connections.
void Server::end()
{
    sd1 = 0;
    sd2 = 0;
    m_count = 0;
    m_obj.remove("next");
    m_obj.remove("now");
    m_obj.remove("opponent");
    m_obj.remove("previous");
    m_obj.remove("sinked");
    QJsonDocument o(m_obj);
    sock1->write(o.toJson());
    sock2->write(o.toJson());
    qDebug() << "GAME ENDED" << m_obj;
}


//  If QJsonObject contains value "done" then we have to add
//  new message to status label in dialog, disconnect our
//  sockets for players and set them to nullptr.
void Server::done()
{
    qDebug() << "done " << m_obj;
    ui->status->setText("There are two open spots for playing");
    if (m_obj.contains("quit")) {
        if (m_obj.value("quit") == m_name1) {
            disconnect(sock2, SIGNAL(aboutToClose()), sock2, SLOT(deleteLater()));
            sd2 = 0;
            sock2 = nullptr;
        } else {
            disconnect(sock1, SIGNAL(aboutToClose()), sock1, SLOT(deleteLater()));
            sd1 = 0;
            sock1 = nullptr;
        }
        m_obj.remove("quit");
        m_obj.remove("done");
    } else {
        disconnect(sock1, SIGNAL(aboutToClose()), sock1, SLOT(deleteLater()));
        disconnect(sock2, SIGNAL(aboutToClose()), sock2, SLOT(deleteLater()));
        sock1 = nullptr;
        sock2 = nullptr;
    }
}

void Server::quit(QTcpSocket *sock)
{
    qDebug() << m_obj;
    QJsonDocument o(m_obj);
    if (sock->socketDescriptor() == sd1) {
        disconnect(sock1, SIGNAL(disconnected()), sock1, SLOT(deleteLater()));
        sd1 = 0;
        sock1 = nullptr;
        sock2->write(o.toJson());
    } else if (sock->socketDescriptor() == sd2) {
        disconnect(sock2, SIGNAL(disconnected()), sock2, SLOT(deleteLater()));
        sd2 = 0;
        sock2 = nullptr;
        sock1->write(o.toJson());
    }
}
