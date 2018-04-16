#include "Minkowski.h"

Minkowski_Set::Minkowski_Set()
{
    m_nearest_point = vec2(0, 0);
    m_nearest_point_length = 0;
}

Minkowski_Set::Minkowski_Set(std::vector<vec2> vec_vertex)
{
    m_vec_vertex = vec_vertex;
    m_nearest_point = vec2(0, 0);
    m_nearest_point_length = 0;
}

Minkowski_Set::~Minkowski_Set() {}

///this = -this
Minkowski_Set Minkowski_Set::operator-()
{
    Minkowski_Set temp_m_set;
    for(unsigned int i = 0; i < m_vec_vertex.size(); i++)
    {
        vec2 temp_vertex(-m_vec_vertex[i].a, -m_vec_vertex[i].b);
        temp_m_set.m_vec_vertex.push_back(temp_vertex);
    }
    return temp_m_set;
}

///M1 = this + M2
Minkowski_Set Minkowski_Set::operator+(Minkowski_Set m_set)
{
    Minkowski_Set temp_m_set;
    for(unsigned int i = 0; i < m_vec_vertex.size(); i++)
    {
        for(unsigned int j = 0; j < m_set.m_vec_vertex.size(); j++)
        {
            vec2 temp_vertex(m_vec_vertex[i].a + m_set.m_vec_vertex[j].a, m_vec_vertex[i].b + m_set.m_vec_vertex[j].b);

            unsigned int k = 0;
            do{k++;} while((k < temp_m_set.m_vec_vertex.size()) && ((temp_m_set.m_vec_vertex[k].a != temp_vertex.a) || (temp_m_set.m_vec_vertex[k].b != temp_vertex.b)));
            if(k >= temp_m_set.m_vec_vertex.size())
                temp_m_set.m_vec_vertex.push_back(temp_vertex);
        }
    }
    return temp_m_set;
}

///M1 = this - M2
Minkowski_Set Minkowski_Set::operator-(Minkowski_Set m_set)
{
    return *this + (-m_set);
}

///Finding the (0, 0) in set's difference
bool Minkowski_Set::Intersect(Minkowski_Set m_set)
{
    Minkowski_Set temp_m_set = *this - m_set;
    vector<vec2> vec_vertex_convex;
    bool intersection_state = false;
    if(Convex(temp_m_set.m_vec_vertex, vec_vertex_convex))
    {
        intersection_state = PointCollisionTest(vec_vertex_convex, vec2(0, 0));
        if(intersection_state)
            m_nearest_point = GetNearestPoint(vec_vertex_convex, m_nearest_point_length);
    }


    return intersection_state;
}

vec2 Minkowski_Set::GetNearestPoint(vector<vec2> vec_vertex, float &length)
{
    vec2 p1 = vec_vertex[vec_vertex.size() - 1];
    vec2 p2 = vec_vertex[0];

    float a = p1.b - p2.b;
    float b = p2.a - p1.a;
    float c = (p1.a * p2.b) - (p2.a * p1.b);
    ///ax + bx + c = 0

    float minx = -(a * c) / ((a * a) + (b * b));
    float miny = -(b * c) / ((a * a) + (b * b));
    float minvecDist = (minx * minx) + (miny * miny);

    for(auto i = 0; i < vec_vertex.size() - 1; i++)
    {
        a = vec_vertex[i].b - vec_vertex[i + 1].a;
        b = vec_vertex[i + 1].a - vec_vertex[i].a;
        c = (vec_vertex[i].a * vec_vertex[i + 1].b) - (vec_vertex[i + 1].a * vec_vertex[i].b);
        float x = -(a * c) / ((a * a) + (b * b));
        float y = -(b * c) / ((a * a) + (b * b));
        float vecDist = (x * x) + (y * y);
        if(vecDist < minvecDist)
        {
            minvecDist = vecDist;
            minx = x;
            miny = y;
        }
    }
    length = sqrt(minvecDist);
    return vec2(minx, miny);
}
