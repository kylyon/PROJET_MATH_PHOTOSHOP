#ifndef POLYGON_H
#define POLYGON_H
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Point.h"
#include "Color.h"

using std::vector;

class Poly
{
    public:
        Poly(Color c);
        Poly();
        virtual ~Poly();

        vector<Point> Getpoints() { return m_points; }
        vector<Point> *GetpointsPointer() { return &m_points; }
        Color Getcolor() { return color; }
        void Addpoint(Point p);
        void display();
        bool isHoraire();
        float GetXMin();
        float GetXMax();
        float GetYMin();
        float GetYMax();
        bool contains(Point p);
        Point GetMiddle();
        void sortedPoints();

    protected:

    private:
        vector<Point> m_points;
        Color color;
        float xMax, xMin, yMax, yMin;
};

#endif // POLYGON_H
