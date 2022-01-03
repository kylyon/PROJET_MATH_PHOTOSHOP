#include "Color.h"

Color::Color()
{
    Color(1.0f,1.0f,1.0f,1.0f);
}

Color::Color(float red, float green, float blue, float alpha)
{
    this->m_red = red;
    this->m_green = green;
    this->m_blue = blue;
    this->m_alpha = alpha;
}

Color::Color(float red, float green, float blue)
{
    Color(red, green, blue, 1.0f);
}


Color::~Color()
{
    //dtor
}
