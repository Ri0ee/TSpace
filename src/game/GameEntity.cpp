#include "GameEntity.h"

TGameEntity::TGameEntity(string entity_name, TPolygon polygon, vec2 origin, vec2 acceleration, string sprite_name)
{
    m_name = entity_name;
    m_x = origin.a;
    m_y = origin.b;
    polygon.m_sprite_name = sprite_name;
    m_vecPolygon.push_back(polygon);

    m_velocity_x = 0;
    m_velocity_y = 0;
    m_acceleration_x = acceleration.a;
    m_acceleration_y = acceleration.b;

    FindBoudaries();
    m_vecPolygon[0].Rotate(M_PI_2, m_width, m_height);
}

TGameEntity::~TGameEntity()
{
    m_vecPolygon.clear();
}

void TGameEntity::AddShape(TPolygon polygon)
{
    m_vecPolygon.push_back(polygon);
}

void TGameEntity::Draw()
{
    for(unsigned int i = 0; i < m_vecPolygon.size(); i++)
    {
        m_vecPolygon[i].Draw(m_x, m_y);
    }
}

void TGameEntity::FindBoudaries()
{
    float MaxW = m_vecPolygon[0].m_vec_vertex_out.m_vec_vertex[0].a, MaxH = m_vecPolygon[0].m_vec_vertex_out.m_vec_vertex[0].b;
    float MinW = m_vecPolygon[0].m_vec_vertex_out.m_vec_vertex[0].a, MinH = m_vecPolygon[0].m_vec_vertex_out.m_vec_vertex[0].b;
    for(unsigned int i = 0; i < m_vecPolygon.size(); i++)
    {
        for(unsigned int j = 0; j < m_vecPolygon[i].m_vec_vertex_out.m_vec_vertex.size(); j++)
        {
            MaxW = max(m_vecPolygon[i].m_vec_vertex_out.m_vec_vertex[j].a, MaxW);
            MaxH = max(m_vecPolygon[i].m_vec_vertex_out.m_vec_vertex[j].b, MaxH);
            MinW = min(m_vecPolygon[i].m_vec_vertex_out.m_vec_vertex[j].a, MinW);
            MinH = min(m_vecPolygon[i].m_vec_vertex_out.m_vec_vertex[j].b, MinH);
        }
    }
    m_width = MaxW;
    m_height = MaxH;
    m_x += MinW;
    m_y += MinH;
}
