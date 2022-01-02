#include "Point.h"

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

Point::~Point()
{
    //dtor
}
