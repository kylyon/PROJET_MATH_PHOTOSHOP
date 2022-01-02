#ifndef POINT_H
#define POINT_H
#include <GL/glut.h>

class Point
{
    public:
        Point();
        Point(float x, float y);
        virtual ~Point();

        float Getx() { return m_x; }
        void Setx(float val) { m_x = val; }
        float Gety() { return m_y; }
        void Sety(float val) { m_y = val; }

    protected:

    private:
        float m_x;
        float m_y;
};

#endif // POINT_H
