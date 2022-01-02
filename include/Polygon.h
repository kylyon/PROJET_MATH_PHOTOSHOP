#ifndef POLYGON_H
#define POLYGON_H
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Point.h"

using std::vector;

class Poly
{
    public:
        Poly();
        virtual ~Poly();

        vector<Point> Getpoints() { return m_points; }
        void Addpoint(Point p);

    protected:

    private:
        vector<Point> m_points;
};

#endif // POLYGON_H
