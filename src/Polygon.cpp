#include "Polygon.h"
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <limits>
#include <cmath>
#include "Point.h"
#include "Color.h"

using namespace std;


Poly::Poly(Color c)
{
    //this->m_points = new vector<Point>();
    xMin, yMin = std::numeric_limits<float>::max();
    xMax, yMax = std::numeric_limits<float>::min();
    color = c;
}

float Poly::GetXMin() {
    return xMin;
}

float Poly::GetXMax() {
    return xMax;
}

float Poly::GetYMin() {
    return yMin;
}

float Poly::GetYMax() {
    return yMax;
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

    //printf("%0.2f - %0.2f - %0.2f - %0.2f", xMax, xMin, yMax, yMin);

}

void Poly::display()
{
    //printf("%d\n",m_points.size());
    glBegin(GL_POLYGON);
    for(int i = 0; i < m_points.size(); i++)
    {
        glColor3f(color.Getred(), color.Getgreen(), color.Getblue());
        float x = m_points[i].Getx();
        float y = m_points[i].Gety();
        glVertex2f(x,y);
    }
    glEnd();
    //printf("\nis Horaire : %d", this->isHoraire());
}

bool Poly::isHoraire()
{
    int res = 0;
    for(int i = 0; i < m_points.size(); i++)
    {
        res += (m_points[(i + 1) % m_points.size()].Getx() - m_points[i].Getx()) * (m_points[(i + 1) % m_points.size()].Gety() + m_points[i].Gety());
        //printf("\n(%f,%f)(%f,%f) = %f",m_points[(i + 1) % m_points.size()].Getx() , m_points[i].Getx(),m_points[(i + 1) % m_points.size()].Gety() , m_points[i].Gety(), (m_points[(i + 1) % m_points.size()].Getx() - m_points[i].Getx()) * (m_points[(i + 1) % m_points.size()].Gety() + m_points[i].Gety()));
    }

    return res > 0;
}

bool Poly::contains(Point p)
{
    for(int i = 0; i < m_points.size(); i++)
    {
        if(m_points[i].isEqual(p))
        {
            return true;
        }
    }
    return false;
}

Point Poly::GetMiddle()
{
    float x, y, a;
    x = 0; y = 0; a = 0;
    for(int i = 0; i < m_points.size(); i++)
    {
        a += (m_points[i].Getx() * m_points[(i+1)%m_points.size()].Gety()) - (m_points[(i+1)%m_points.size()].Getx() * m_points[i].Gety());
        x += ((m_points[i].Getx() * m_points[(i+1)%m_points.size()].Gety()) - (m_points[(i+1)%m_points.size()].Getx() * m_points[i].Gety())) * (m_points[i].Getx() + m_points[(i+1)%m_points.size()].Getx());
        y +=((m_points[i].Getx() * m_points[(i+1)%m_points.size()].Gety()) - (m_points[(i+1)%m_points.size()].Getx() * m_points[i].Gety())) * (m_points[i].Gety() + m_points[(i+1)%m_points.size()].Gety());
    }

    x /= 6 * a;
    y /= 6 * a;

    return Point(x, y);
}

void Poly::sortedPoints()
{
    Point origin= GetMiddle();
    Point vectorOriginToFirst = Point(m_points[0].Getx() - origin.Getx() , m_points[0].Gety() -  origin.Gety());
    float normalOF = sqrt(pow(vectorOriginToFirst.Getx(), 2) + pow(vectorOriginToFirst.Gety(), 2));

    vector<Point> temp;
    temp.push_back(m_points[0]);
    for(int i = 1; i < m_points.size(); i++)
    {
        int minimum = 10000;
        int index = 0;
        //printf("\nOF : ( %f , %f)->( %f, %f) - OX : ( %f , %f)->( %f, %f) = theta : %f - %f",origin.Getx(), origin.Gety(),  m_points[0].Getx(), m_points[0].Gety(), origin.Getx(), origin.Gety(), m_points[i].Getx(), m_points[i].Gety(), xp,theta);

        for(int j = i; j < m_points.size(); j++)
        {

            Point vectorOriginToX = Point(m_points[j].Getx() - origin.Getx() , m_points[j].Gety() -  origin.Gety());
            float normalOX = sqrt(pow(vectorOriginToX.Getx(), 2) + pow(vectorOriginToX.Gety(), 2));

            float dotOFOX = (vectorOriginToFirst.Getx() * vectorOriginToX.Getx()) + (vectorOriginToFirst.Gety() * vectorOriginToX.Gety());

            float theta = (dotOFOX)/(normalOF * normalOX);
            theta = acos(theta) * 180/M_PI;

            float xp = vectorOriginToFirst.Getx() * vectorOriginToX.Gety() - vectorOriginToFirst.Gety() * vectorOriginToX.Getx();

            if(xp > 0 )
            {
                theta = 360-theta;
            }

            if(theta < minimum)
            {
                minimum = theta;
                index = j;
            }
        }
        temp.push_back(m_points[index]);
        m_points[index] = m_points[i];
    }

    m_points = temp;


}

Poly::~Poly()
{
    //dtor
}
