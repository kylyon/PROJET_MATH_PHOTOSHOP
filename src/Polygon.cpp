#include "Polygon.h"
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <limits>
#include "Point.h"
#include "Color.h"

using std::vector;


Poly::Poly(Color c)
{
    //this->m_points = new vector<Point>();
    xMin, yMin = std::numeric_limits<float>::max();
    xMax, yMax = std::numeric_limits<float>::min();
    color = c;
}

void Poly::Addpoint(Point p)
{
    this->m_points.push_back(p);

    if(p.Getx() > xMax)
    {
        xMax = p.Getx();
    }

    if(p.Getx() < xMin)
    {
        xMin = p.Getx();
    }

    if(p.Gety() > yMax)
    {
        yMax = p.Gety();
    }

    if(p.Gety() < yMin)
    {
        yMin = p.Gety();
    }

    printf("%0.2f - %0.2f - %0.2f - %0.2f", xMax, xMin, yMax, yMin);

}

void Poly::display()
{
    printf("%d\n",m_points.size());
    glBegin(GL_POLYGON);
    for(int i = 0; i < m_points.size(); i++)
    {
        glColor3f(color.Getred(), color.Getgreen(), color.Getblue());
        float x = m_points[i].Getx();
        float y = m_points[i].Gety();
        glVertex2f(x,y);
    }
    glEnd();
}

Poly::~Poly()
{
    //dtor
}
