#include "machine.h"
#include <QDebug>
#include <QTime>
#include <iostream>
#define MAX_DIMENSION 10
Machine::Machine()
{
   m_matrixMachine = new std::vector<std::vector<bool>>(MAX_DIMENSION, std::vector<bool>(MAX_DIMENSION, false));
   m_attackedSquares = new std::vector<QPoint>;

   m_firstGuessInARow = false;
   m_haveNewGuesses = false;

   m_goUp = false;
   m_goDown = false;
   m_goLeft = false;
   m_goRight = false;

   m_wentUp = false;
   m_wentDown = false;
   m_wentLeft = false;
   m_wentRight = false;

   bool k = initMachine();
}

bool Machine::getValueOnPosition(int x, int y) const
{
    return m_matrixMachine->at(x).at(y);
}

int Machine::getCorrectGuesses() const
{
    return m_countCorrectGuessesMachine;
}

void Machine::setCorrectGuesses(int number)
{
    m_countCorrectGuessesMachine = number;
}

std::vector<QPoint> *Machine::getAttackedSquares() const
{
    return m_attackedSquares;
}

void Machine::setAttackedSquares(std::vector<QPoint> newAttackedSquares)
{
    *m_attackedSquares = newAttackedSquares;
}

bool Machine::givenSquareIsAlreadyAttacked(QPoint sq) const
{
    int n = m_attackedSquares->size();
    if (n==0)
        return false;

    for (QPoint point : *m_attackedSquares) {
        if (point.x() == sq.x() && point.y() == sq.y())
            return true;
    }

    return false;

}

bool Machine::IWon()//ship length goes from 2 to 5, the machine won if all ships sunk
{
    if (m_countCorrectGuessesMachine == (2 + 3 + 4 + 5))
        return true;

    return false;
}

bool Machine::initMachine()//initialization of the matrix which preserves the positions of ships. Ship positions are randomly chosen.
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    bool withoutOverlap = true;
    //loop of ships, ship length goes from 2 to 5
    for(int i = 2; i<6; i++){
        int x = qrand() % MAX_DIMENSION;
        int y = qrand() % MAX_DIMENSION;

        if(qrand() % MAX_DIMENSION <= 5){//vertical setting of the ship
            if(x+i<MAX_DIMENSION){
                for(int j = 0;j<i;j++){
                    if(m_matrixMachine->at(x+j).at(y) == 1)
                        withoutOverlap = false;
                    m_matrixMachine->at(x+j).at(y) = 1;
                }
            }
            else{
                for(int j = 0;j<i;j++){
                    if(m_matrixMachine->at(x-j).at(y) == 1)
                        withoutOverlap = false;
                    m_matrixMachine->at(x-j).at(y) = 1;
                }
            }

        }else{//horizontal setting of the ship
            if(y+i<MAX_DIMENSION){
                for(int j = 0;j<i;j++){
                    if(m_matrixMachine->at(x).at(y+j) == 1)
                        withoutOverlap = false;
                    m_matrixMachine->at(x).at(y+j) = 1;
                }
            }
            else{
                for(int j = 0;j<i;j++){
                   if(m_matrixMachine->at(x).at(y-j) == 1)
                        withoutOverlap = false;
                   m_matrixMachine->at(x).at(y-j) = 1;
                }
            }
        }
    }
      //print the matrix value for test
      for(int i = 0; i < MAX_DIMENSION; i++){
          for(int j = 0; j < MAX_DIMENSION; j++){
              std::cout << " "<< m_matrixMachine->at(i).at(j);
          }
          std::cout <<std::endl;
      }

     return withoutOverlap;
}

void Machine::resetInit()
{
    for(int i = 0; i < MAX_DIMENSION; i++){
        for(int j = 0; j < MAX_DIMENSION; j++){
            m_matrixMachine->at(i).at(j) = 0;
        }
    }
}
bool Machine::getIsFirstGuessInARow() const
{
    return m_firstGuessInARow;
}

void Machine::setIsFirstGuessInARow(bool statement)
{
    m_firstGuessInARow = statement;
}

bool Machine::getHaveNewGuesses() const
{
    return m_haveNewGuesses;
}

void Machine::setHaveNewGuesses(bool statement)
{
    m_haveNewGuesses = statement;
}

bool Machine::getGoUp() const
{
    return m_goUp;
}

void Machine::setGoUp(bool statement)
{
    m_goUp = statement;
}

bool Machine::getGoDown() const
{
    return m_goDown;
}

void Machine::setGoDown(bool statement)
{
    m_goDown = statement;
}

bool Machine::getGoLeft() const
{
    return m_goLeft;
}

void Machine::setGoLeft(bool statement)
{
    m_goLeft = statement;
}

bool Machine::getGoRight() const
{
    return m_goRight;
}

void Machine::setGoRight(bool statement)
{
    m_goRight = statement;
}

bool Machine::getWentUp() const
{
    return m_wentUp;
}

void Machine::setWentUp(bool statement)
{
    m_wentUp = statement;
}

bool Machine::getWentDown() const
{
    return m_wentDown;
}

void Machine::setWentDown(bool statement)
{
    m_wentDown = statement;
}

bool Machine::getWentLeft() const
{
    return m_wentLeft;
}

void Machine::setWentLeft(bool statement)
{
    m_wentLeft = statement;
}

bool Machine::getWentRight() const
{
    return m_wentRight;
}

void Machine::setWentRight(bool statement)
{
    m_wentRight = statement;
}
