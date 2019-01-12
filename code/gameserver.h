#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Gameserver;
}

class Gameserver : public QDialog
{
    Q_OBJECT

public:
    explicit Gameserver(QWidget *parent = 0);
    ~Gameserver();
    Main *m;

private slots:
    void on_Play_clicked();

    void on_Close_clicked();

private:
    Ui::Gameserver *ui;
};

#endif // GAMESERVER_H
