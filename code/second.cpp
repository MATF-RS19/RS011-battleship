#include "second.h"
#include "ui_second.h"

Second::Second(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Second)
{
    ui->setupUi(this);
    QPixmap pix(":/battleship1.jpg");
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, pix);
    this->setPalette(palette);
}

Second::~Second()
{
    delete ui;
}

void Second::on_Play_clicked()
{
    hide();
    m = new Main(this);
    m->show();
}

void Second::on_Close_clicked()
{
    this->close();
}
