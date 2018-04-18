#pragma once

#include <vector>
#include <string>
#include <math.h>
#include <algorithm>

#include "Structures.h"
#include "Minkowski.h"
#include "Render.h"
#include "polypartition.h"
#include <list>
#include "PointCollision.h"

using std::string;

class TPolygon
{
    public:
        TPolygon();
        TPolygon(int r, int g, int b, int a);
        virtual ~TPolygon();

        void PrintVerticesOut();
        void addVertex(vec2 vertex);
        void addVertex(float x, float y);
        void Draw(float x0, float y0);

        bool FindForwardCollsionMinkowski(TPolygon second_polygon, float &penetrationDepth);
        bool FindForwardCollsionPC(TPolygon second_polygon);

        color m_color;
        Minkowski_Set m_vec_vertex;
};
