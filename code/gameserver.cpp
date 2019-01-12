#include "gameserver.h"
#include "ui_gameserver.h"

Gameserver::Gameserver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gameserver)//setting the background for the first window
{
    ui->setupUi(this);
    QPixmap pix(":/battleship1.jpg");
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, pix);
    this->setPalette(palette);
}

Gameserver::~Gameserver()
{
    delete ui;
}

void Gameserver::on_Play_clicked()//hides the first window and shows the main window
{
    hide();
    m = new Main(this);
    m->show();
    m->init();
}

void Gameserver::on_Close_clicked()//closes the first window
{
    this->close();
}
