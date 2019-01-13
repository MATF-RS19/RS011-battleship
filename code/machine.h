#ifndef MACHINE_H
#define MACHINE_H
#include <QPoint>
#include <vector>

class Machine
{
public:
    Machine();
    bool getValueOnPosition(int x, int y) const;
    void attackOnPosition(int x, int y);
    int getCorrectGuesses() const;
    void setCorrectGuesses(int number);

    std::vector<QPoint>* getAttackedSquares() const;
    void setAttackedSquares(std::vector<QPoint> newAttackedSquares);
    bool givenSquareIsAlreadyAttacked(QPoint sq) const;
    bool IWon();

    bool getIsFirstGuessInARow() const;
    void setIsFirstGuessInARow(bool statement);
    bool getHaveNewGuesses() const;
    void setHaveNewGuesses(bool statement);

    bool getGoUp() const;
    void setGoUp(bool statement);
    bool getGoDown() const;
    void setGoDown(bool statement);
    bool getGoLeft() const;
    void setGoLeft(bool statement);
    bool getGoRight() const;
    void setGoRight(bool statement);

    bool getWentUp() const;
    void setWentUp(bool statement);
    bool getWentDown() const;
    void setWentDown(bool statement);
    bool getWentLeft() const;
    void setWentLeft(bool statement);
    bool getWentRight() const;
    void setWentRight(bool statement);

private:
    std::vector<std::vector<bool>> *m_matrixMachine;
    std::vector<QPoint> *m_attackedSquares;
    int m_countCorrectGuessesMachine = 0;
    bool initMachine();
    void resetInit();

    bool m_firstGuessInARow;
    bool m_haveNewGuesses;

    bool m_goUp;
    bool m_goDown;
    bool m_goLeft;
    bool m_goRight;

    bool m_wentUp;
    bool m_wentDown;
    bool m_wentLeft;
    bool m_wentRight;
};

#endif // MACHINE_H
