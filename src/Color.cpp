#include "Color.h"

Color::Color()
{
    Color(1.0f,1.0f,1.0f);
}

Color::Color(float red, float green, float blue)
{
    this->m_red = red;
    this->m_green = green;
    this->m_blue = blue;
}


Color::~Color()
{
    //dtor
}
