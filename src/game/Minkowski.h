#pragma once

#include <vector>

#include "Structures.h"
#include "ConvexHull.h"
#include "PointCollision.h"
#include "Utility.h"

using std::vector;

class Minkowski_Set
{
    public:
        Minkowski_Set();
        Minkowski_Set(vector<vec2> vec_vertex);
        virtual ~Minkowski_Set();

        Minkowski_Set operator-();
        Minkowski_Set operator-(Minkowski_Set m_set);
        Minkowski_Set operator+(Minkowski_Set m_set);

        bool Intersect(Minkowski_Set m_set);
        vec2 GetNearestPoint(vector<vec2> vec_vertex, float &length);

        vec2 m_nearest_point;
        float m_nearest_point_length;

        vector<vec2> m_vec_vertex;
};
