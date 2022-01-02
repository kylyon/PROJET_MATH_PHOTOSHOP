#include "Polygon.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Point.h"

using std::vector;

Poly::Poly()
{
    //this->m_points = new vector<Point>();
}

void Poly::Addpoint(Point p)
{
    this->m_points.push_back(p);
}

Poly::~Poly()
{
    //dtor
}
