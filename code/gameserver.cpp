#include "gameserver.h"
#include "ui_gameserver.h"

GameServer::GameServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameServer)//setting the background for the first window
{
    ui->setupUi(this);
    QPixmap pix(":/battleship1.jpg");
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, pix);
    this->setPalette(palette);

    this->setWindowTitle("battleship");
    ui->twoPlayers->setEnabled(false);
    connect(ui->twoPlayers, SIGNAL(clicked(bool)), this, SLOT(onTwoPlayers()));
    connect(ui->singlePlayer, SIGNAL(clicked(bool)), this, SLOT(onSinglePlayer()));
    connect(ui->close, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->lineEdit1, SIGNAL(textEdited(QString)), this, SLOT(enable()));
    connect(ui->lineEdit3, SIGNAL(textEdited(QString)), this, SLOT(enable()));

}

GameServer::~GameServer()
{
    delete ui;
}

int GameServer::getMode() const
{
    return m_mode;
}

void GameServer::keyPressEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_Escape) {
        QDialog::keyPressEvent(e);
    } else {
        e->ignore();
    }
}

void GameServer::onSinglePlayer()
{
    m_mode = 1;
    this->hide();

    if (ui->lineEdit2->text().trimmed() != "")
        m_name = ui->lineEdit2->text().trimmed();
    else
        m_name = "Player";

    m_player = new Main();
    m_player->singlePlayer(m_name, "Machine");
    m_player->show();
}

void GameServer::onTwoPlayers()
{
    this->hide();
    m_mode = 2;
    m_player = new Main();
    QHostAddress addr;
    addr.setAddress(m_addr);
    if (m_player->connectToGameServer(m_name, addr, 4242))
        qDebug() << "Connected";
    m_player->show();
}

void GameServer::onClose()//closes the first window
{
    this->close();
}

void GameServer::enable()
{
    if (ui->lineEdit1->text().trimmed() != "") {
        if (ui->lineEdit3->text().trimmed() != "") {
            ui->twoPlayers->setEnabled(true);
            m_name = ui->lineEdit1->text().trimmed();
            m_addr = ui->lineEdit3->text().trimmed();
        }
    } else if (ui->lineEdit3->text().trimmed() != "") {
        if (ui->lineEdit1->text().trimmed() != "") {
            ui->twoPlayers->setEnabled(true);
            m_name = ui->lineEdit1->text().trimmed();
            m_addr = ui->lineEdit3->text().trimmed();
        }
    }
    if (ui->lineEdit3->text().trimmed() == "" || ui->lineEdit1->text().trimmed() == "") {
        ui->twoPlayers->setEnabled(false);
    }
}

