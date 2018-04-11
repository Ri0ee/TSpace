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
        void Rotate(float angle, float x0, float y0);
        void RotateBase(float angle, float x0, float y0);

        bool FindForwardCollsion(TPolygon second_polygon, float deltaTime, float &penetrationDepth);

        color m_color;
        Minkowski_Set m_vec_vertex_base;
        Minkowski_Set m_vec_vertex_out;
        bool m_is_changed;
};
