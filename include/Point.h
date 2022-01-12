#ifndef POINT_H
#define POINT_H
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

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
        bool isEqual(Point p);

    protected:

    private:
        float m_x;
        float m_y;
};

#endif // POINT_H
