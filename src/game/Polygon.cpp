#include "Polygon.h"
#include <iostream>

TPolygon::TPolygon()
{
    m_vec_vertex.m_vec_vertex.clear();
    m_color.r = 100;
    m_color.g = 100;
    m_color.b = 100;
    m_color.a = 100;
}

TPolygon::TPolygon(int r, int g, int b, int a)
{
    m_vec_vertex.m_vec_vertex.clear();
    m_color.r = r;
    m_color.g = g;
    m_color.b = b;
    m_color.a = a;
}

TPolygon::~TPolygon() {}

void TPolygon::PrintVerticesOut()
{
    std::cout << "Printing polygon out: " << std::endl;
    for(int i = 0; i < m_vec_vertex.m_vec_vertex.size(); i++)
        std::cout << "(" << m_vec_vertex.m_vec_vertex[i].a << ";" << m_vec_vertex.m_vec_vertex[i].b << ")" << std::endl;
    std::cout << "Finished printing polygon out." << std::endl;
}

void TPolygon::addVertex(vec2 vertex)
{
    m_vec_vertex.m_vec_vertex.push_back(vertex);
}

void TPolygon::addVertex(float x, float y)
{
    m_vec_vertex.m_vec_vertex.push_back(vec2(x, y));
}

void TPolygon::Draw(float X0, float Y0)
{
    TPPLPartition pp;
    TPPLPoly temp_polygon;
    temp_polygon.Init(m_vec_vertex.m_vec_vertex.size());

    for(int i = 0; i < m_vec_vertex.m_vec_vertex.size(); i++)
    {
        temp_polygon[i].x = m_vec_vertex.m_vec_vertex[i].a;
        temp_polygon[i].y = m_vec_vertex.m_vec_vertex[i].b;
    }

    std::list<TPPLPoly> triangles;
    pp.Triangulate_OPT(&temp_polygon, &triangles);

    for(auto it = triangles.begin(); it != triangles.end(); it++)
    {
        std::vector<vec2> temp_vector;
        for(int i = 0; i < it->GetNumPoints(); i++)
            temp_vector.push_back(vec2(it->GetPoint(i).x, it->GetPoint(i).y));
        graphics::DrawPolygon(temp_vector, m_color, vec2(X0, Y0), false, 2);
    }
}

bool TPolygon::FindForwardCollsionMinkowski(TPolygon second_polygon, float& penetration_depth)
{
    TPPLPartition pp;

    TPPLPoly temp_poly_1; ///this polygon
    temp_poly_1.Init(m_vec_vertex.m_vec_vertex.size());

    TPPLPoly temp_poly_2; ///second polygon
    temp_poly_2.Init(second_polygon.m_vec_vertex.m_vec_vertex.size());

    for(unsigned int i = 0; i < m_vec_vertex.m_vec_vertex.size(); i++) ///transferring verticies from TPolygon to TPPLPoly
    {
        temp_poly_1[i].x = m_vec_vertex.m_vec_vertex[i].a;
        temp_poly_1[i].y = m_vec_vertex.m_vec_vertex[i].b;
    }

    for(unsigned int i = 0; i < second_polygon.m_vec_vertex.m_vec_vertex.size(); i++) ///transferring verticies from TPolygon to TPPLPoly
    {
        temp_poly_2[i].x = second_polygon.m_vec_vertex.m_vec_vertex[i].a;
        temp_poly_2[i].y = second_polygon.m_vec_vertex.m_vec_vertex[i].b;
    }

    std::list<TPPLPoly> p1;
    std::list<TPPLPoly> p2;
    pp.Triangulate_OPT(&temp_poly_1, &p1);
    pp.Triangulate_OPT(&temp_poly_2, &p2);

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
    temp_poly_1.Init(m_vec_vertex.m_vec_vertex.size());

    TPPLPoly temp_poly_2; ///second polygon
    temp_poly_2.Init(second_polygon.m_vec_vertex.m_vec_vertex.size());

    for(unsigned int i = 0; i < m_vec_vertex.m_vec_vertex.size(); i++) ///transferring verticies from TPolygon to TPPLPoly
    {
        temp_poly_1[i].x = m_vec_vertex.m_vec_vertex[i].a;
        temp_poly_1[i].y = m_vec_vertex.m_vec_vertex[i].b;
    }

    for(unsigned int i = 0; i < second_polygon.m_vec_vertex.m_vec_vertex.size(); i++) ///transferring verticies from TPolygon to TPPLPoly
    {
        temp_poly_2[i].x = second_polygon.m_vec_vertex.m_vec_vertex[i].a;
        temp_poly_2[i].y = second_polygon.m_vec_vertex.m_vec_vertex[i].b;
    }

    std::list<TPPLPoly> p1;
    std::list<TPPLPoly> p2;
    pp.Triangulate_OPT(&temp_poly_1, &p1);
    pp.Triangulate_OPT(&temp_poly_2, &p2);

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
