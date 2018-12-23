#ifndef SECOND_H
#define SECOND_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Second;
}

class Second : public QDialog
{
    Q_OBJECT

public:
    explicit Second(QWidget *parent = 0);
    ~Second();

private slots:
    void on_Play_clicked();

    void on_Close_clicked();

private:
    Ui::Second *ui;
    Main *m;
};

#endif // SECOND_H
