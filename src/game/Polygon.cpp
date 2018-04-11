#include "Polygon.h"
#include <iostream>

TPolygon::TPolygon()
{
    m_vec_vertex_base.m_vec_vertex.clear();
    m_color.r = 100;
    m_color.g = 100;
    m_color.b = 100;
    m_color.a = 100;
}

TPolygon::TPolygon(int r, int g, int b, int a)
{
    m_vec_vertex_base.m_vec_vertex.clear();
    m_color.r = r;
    m_color.g = g;
    m_color.b = b;
    m_color.a = a;
}

TPolygon::~TPolygon() {}

void TPolygon::PrintVerticesOut()
{
    std::cout << "Printing polygon out: " << std::endl;
    for(int i = 0; i < m_vec_vertex_out.m_vec_vertex.size(); i++)
        std::cout << "(" << m_vec_vertex_out.m_vec_vertex[i].a << ";" << m_vec_vertex_out.m_vec_vertex[i].b << ")" << std::endl;
    std::cout << "Finished printing polygon out." << std::endl;
}

void TPolygon::addVertex(vec2 vertex)
{
    m_vec_vertex_base.m_vec_vertex.push_back(vertex);
    m_vec_vertex_out.m_vec_vertex.push_back(vertex);
}

void TPolygon::addVertex(float x, float y)
{
    m_vec_vertex_base.m_vec_vertex.push_back(vec2(x, y));
    m_vec_vertex_out.m_vec_vertex.push_back(vec2(x, y));
}

void TPolygon::Draw(float X0, float Y0)
{
    ///Make triangulation here
    TPPLPartition pp;
    TPPLPoly temp_polygon;
    temp_polygon.Init(m_vec_vertex_out.m_vec_vertex.size());

    for(int i = 0; i < m_vec_vertex_out.m_vec_vertex.size(); i++)
    {
        temp_polygon[i].x = m_vec_vertex_out.m_vec_vertex[i].a;
        temp_polygon[i].y = m_vec_vertex_out.m_vec_vertex[i].b;
    }

    std::list<TPPLPoly> result;
    pp.ConvexPartition_HM(&temp_polygon, &result);

    for(auto it = result.begin(); it != result.end(); it++)
    {
        std::vector<vec2> temp_vector(it->GetNumPoints());
        for(int i = 0; i < it->GetNumPoints(); i++)
        {
            temp_vector.push_back(vec2(it->GetPoint(i).x, it->GetPoint(i).y));
        }
        graphics::DrawPolygon(temp_vector, m_color, vec2(X0, Y0), false, 2);
    }
}

void TPolygon::RotateBase(float angle, float X0, float Y0)
{
    float Ix = cos(angle);
    float Iy = sin(angle);
    float Jx = -sin(angle);
    float Jy = cos(angle);
    for(unsigned int i = 0; i < m_vec_vertex_base.m_vec_vertex.size(); i++)
    {
        float X = m_vec_vertex_base.m_vec_vertex[i].a - (X0 / 2);
        float Y = m_vec_vertex_base.m_vec_vertex[i].b - (Y0 / 2);
        m_vec_vertex_base.m_vec_vertex[i].a = (X * Ix + Y * Jx) + (X0 / 2);
        m_vec_vertex_base.m_vec_vertex[i].b = (X * Iy + Y * Jy) + (Y0 / 2);
    }
}

void TPolygon::Rotate(float angle, float X0, float Y0)
{
    float Ix = cos(angle);
    float Iy = sin(angle);
    float Jx = -sin(angle);
    float Jy = cos(angle);
    float hw = X0 / 2;
    float hh = Y0 / 2;
    for(unsigned int i = 0; i < m_vec_vertex_base.m_vec_vertex.size(); i++)
    {
        float X = m_vec_vertex_base.m_vec_vertex[i].a - hw;
        float Y = m_vec_vertex_base.m_vec_vertex[i].b - hh;
        m_vec_vertex_out.m_vec_vertex[i].a = (X * Ix + Y * Jx) + hw;
        m_vec_vertex_out.m_vec_vertex[i].b = (X * Iy + Y * Jy) + hh;
    }
}

bool TPolygon::FindForwardCollsion(TPolygon second_polygon, float& penetration_depth)
{
    TPPLPartition pp;

    TPPLPoly temp_poly;
    temp_poly.Init(m_vec_vertex_out.m_vec_vertex.size());
    for(unsigned int i = 0; i < m_vec_vertex_out.m_vec_vertex.size(); i++)
    {
        temp_poly[i].x = m_vec_vertex_out.m_vec_vertex[i].a;
        temp_poly[i].y = m_vec_vertex_out.m_vec_vertex[i].b;
    }

    std::list<TPPLPoly> result;
    pp.ConvexPartition_HM(&temp_poly, &result);

    bool temp_collision_state = false;
    float temp_penetration_depth = 0;
    for(auto it = result.begin(); it != result.end(); it++)
    {
        Minkowski_Set temp_m_set;
        int num_points = it->GetNumPoints();
        for(long i = 0; i < num_points; i++)
            temp_m_set.m_vec_vertex.push_back(vec2(it->GetPoint(i).x, it->GetPoint(i).y));

        temp_collision_state = m_vec_vertex_out.Intersect(temp_m_set);
        if(temp_collision_state)
        {
            temp_penetration_depth = temp_m_set.m_nearest_point_length;
            break;
        }
    }

    penetration_depth = temp_penetration_depth;
    return temp_collision_state;
}
