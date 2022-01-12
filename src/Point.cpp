#include "Point.h"
#include <cmath>

Point::Point(float x, float y)
{
    this->m_x = x;
    this->m_y = y;
}

Point::Point()
{
    this->m_x = 0;
    this->m_y = 0;
}

bool Point::isEqual(Point p)
{
    return m_x == p.Getx() && m_y == p.Gety();
}

Point::~Point()
{
    //dtor
}
