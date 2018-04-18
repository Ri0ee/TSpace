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
    TPPLPartition pp;
    TPPLPoly temp_polygon;
    temp_polygon.Init(m_vec_vertex_out.m_vec_vertex.size());
    temp_polygon.SetOrientation(TPPL_CW);

    for(int i = 0; i < m_vec_vertex_out.m_vec_vertex.size(); i++)
    {
        temp_polygon[i].x = m_vec_vertex_out.m_vec_vertex[i].a;
        temp_polygon[i].y = m_vec_vertex_out.m_vec_vertex[i].b;
        temp_polygon[i].id = i;
    }

    std::list<TPPLPoly> triangles;
    pp.Triangulate_OPT(&temp_polygon, &triangles);

    int numtriangles = triangles.size();

    for(auto it = triangles.begin(); it != triangles.end(); it++)
    {
        std::vector<vec2> temp_vector;
        for(int i = 0; i < it->GetNumPoints(); i++)
            temp_vector.push_back(vec2(it->GetPoint(i).x, it->GetPoint(i).y));
        graphics::DrawPolygon(temp_vector, m_color, vec2(X0, Y0), false, 2);
    }
    graphics::DrawPolygon(m_vec_vertex_out.m_vec_vertex, m_color, vec2(X0, Y0), false, 2);
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

bool TPolygon::FindForwardCollsionMinkowski(TPolygon second_polygon, float& penetration_depth)
{
    TPPLPartition pp;

    TPPLPoly temp_poly_1; ///this polygon
    temp_poly_1.Init(m_vec_vertex_out.m_vec_vertex.size());

    TPPLPoly temp_poly_2; ///second polygon
    temp_poly_2.Init(second_polygon.m_vec_vertex_out.m_vec_vertex.size());

    for(unsigned int i = 0; i < m_vec_vertex_out.m_vec_vertex.size(); i++) ///transferring verticies from TPolygon to TPPLPoly
    {
        temp_poly_1[i].x = m_vec_vertex_out.m_vec_vertex[i].a;
        temp_poly_1[i].y = m_vec_vertex_out.m_vec_vertex[i].b;
    }

    for(unsigned int i = 0; i < second_polygon.m_vec_vertex_out.m_vec_vertex.size(); i++) ///transferring verticies from TPolygon to TPPLPoly
    {
        temp_poly_2[i].x = second_polygon.m_vec_vertex_out.m_vec_vertex[i].a;
        temp_poly_2[i].y = second_polygon.m_vec_vertex_out.m_vec_vertex[i].b;
    }

    /*
    std::cout << "temp_poly_1: " << std::endl;
    for(int i = 0; i < temp_poly_1.GetNumPoints(); i++)
        std::cout << "(" << temp_poly_1[i].x << ";" << temp_poly_1[i].y << ")" << std::endl;

    std::cout << "temp_poly_2: " << std::endl;
    for(int i = 0; i < temp_poly_2.GetNumPoints(); i++)
        std::cout << "(" << temp_poly_2[i].x << ";" << temp_poly_2[i].y << ")" << std::endl;
    */

    std::list<TPPLPoly> p1;
    std::list<TPPLPoly> p2;
    pp.Triangulate_OPT(&temp_poly_1, &p1);
    pp.Triangulate_OPT(&temp_poly_2, &p2);

    /*
    std::cout << "p1 size = " << p1.size() << std::endl;
    for(auto it = p1.begin(); it != p1.end(); it++)
    {
        std::cout << "p1 element: " << std::endl;
        for(int j = 0; j < it->GetNumPoints(); j++)
        {
            std::cout << "(" << it->GetPoint(j).x << ";" << it->GetPoint(j).y << ")" << std::endl;
        }
    }

    std::cout << "p2 size = " << p2.size() << std::endl;
    for(auto it = p2.begin(); it != p2.end(); it++)
    {
        std::cout << "p2 element: " << std::endl;
        for(int j = 0; j < it->GetNumPoints(); j++)
        {
            std::cout << "(" << it->GetPoint(j).x << ";" << it->GetPoint(j).y << ")" << std::endl;
        }
    }
    */

    bool temp_collision_state = false;
    float temp_penetration_depth = 2000000000;
    for(auto it = p1.begin(); it != p1.end(); it++)
    {
        Minkowski_Set temp_m_set;
        for(int i = 0; i < it->GetNumPoints(); i++)
            temp_m_set.m_vec_vertex.push_back(vec2(it->GetPoint(i).x, it->GetPoint(i).y));

        for(auto it_2 = p2.begin(); it_2 != p2.end(); it_2++)
        {
            Minkowski_Set temp_m_set_2;
            for(int i = 0; i < it->GetNumPoints(); i++)
                temp_m_set_2.m_vec_vertex.push_back(vec2(it_2->GetPoint(i).x, it_2->GetPoint(i).y));

            temp_collision_state = temp_m_set.Intersect(temp_m_set_2);
            if(temp_collision_state)
            {
                penetration_depth = temp_m_set.m_nearest_point_length;
                return temp_collision_state;
            }
        }
    }

    penetration_depth = temp_penetration_depth;
    return temp_collision_state;
}

bool TPolygon::FindForwardCollsionPC(TPolygon second_polygon)
{
    TPPLPartition pp;

    TPPLPoly temp_poly_1; ///this polygon
    temp_poly_1.Init(m_vec_vertex_out.m_vec_vertex.size());

    TPPLPoly temp_poly_2; ///second polygon
    temp_poly_2.Init(second_polygon.m_vec_vertex_out.m_vec_vertex.size());

    for(unsigned int i = 0; i < m_vec_vertex_out.m_vec_vertex.size(); i++) ///transferring verticies from TPolygon to TPPLPoly
    {
        temp_poly_1[i].x = m_vec_vertex_out.m_vec_vertex[i].a;
        temp_poly_1[i].y = m_vec_vertex_out.m_vec_vertex[i].b;
    }

    for(unsigned int i = 0; i < second_polygon.m_vec_vertex_out.m_vec_vertex.size(); i++) ///transferring verticies from TPolygon to TPPLPoly
    {
        temp_poly_2[i].x = second_polygon.m_vec_vertex_out.m_vec_vertex[i].a;
        temp_poly_2[i].y = second_polygon.m_vec_vertex_out.m_vec_vertex[i].b;
    }

    std::list<TPPLPoly> p1;
    std::list<TPPLPoly> p2;
//    pp.Triangulate_OPT(&temp_poly_1, &p1);
//    pp.Triangulate_OPT(&temp_poly_2, &p2);
    pp.Triangulate_EC(&temp_poly_1, &p1);
    pp.Triangulate_EC(&temp_poly_2, &p2);

    int pp1size = p1.size();
    int pp2size = p2.size();

    for(auto it_1 = p1.begin(); it_1 != p1.end(); it_1++)
    {
        std::vector<vec2> temp_vector_1;
        for(int i = 0; i < it_1->GetNumPoints(); i++)
            temp_vector_1.push_back(vec2(it_1->GetPoint(i).x, it_1->GetPoint(i).y));

        for(auto it_2 = p2.begin(); it_2 != p2.end(); it_2++)
        {
            std::vector<vec2> temp_vector_2;
            for(int i = 0; i < it_2->GetNumPoints(); i++)
                temp_vector_2.push_back(vec2(it_2->GetPoint(i).x, it_2->GetPoint(i).y));

            for(int i = 0; i < it_2->GetNumPoints(); i++)
            {
                if(PointCollisionTest(temp_vector_1, temp_vector_2[i]))
                    return true;
            }
        }
    }

    return false;
}
