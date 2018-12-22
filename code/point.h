#ifndef POINT_H
#define POINT_H

class Point {
public:
    Point(int x, int y);
    int getX();
    int getY();
private:
    int m_x;
    int m_y;
};

#endif // POINT_H
