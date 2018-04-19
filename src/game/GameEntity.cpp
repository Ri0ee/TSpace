#include "GameEntity.h"

TGameEntity::TGameEntity(int entity_type, const string& sprite_name, TPolygon polygon, vec2 position, vec2 acceleration, int life_time, int max_life_time)
{
    m_sprite_name = sprite_name;
    m_type = entity_type;

    m_x = position.a;
    m_y = position.b;
    m_velocity_x = 0;
    m_velocity_y = 0;
    m_acceleration_x = acceleration.a;
    m_acceleration_y = acceleration.b;

    m_maximum_life = 100;
    m_current_life = m_maximum_life;
    m_maximum_ammo = 30;
    m_current_ammo = m_maximum_ammo;

    m_max_life_time = max_life_time;
    m_life_time = life_time;

    m_shoot_timer = 0;

    m_did_hit = false;
    m_is_destroyed = false;

    m_task.m_is_active = false;
    m_task.m_ax = 0;
    m_task.m_ay = 0;
    m_task.m_current_duration = 0;
    m_task.m_maximum_duration = 100;
    m_task.m_type = 0;
    m_task.m_x = 0;
    m_task.m_y = 0;

    m_bullet_owner_id = -1;

    m_vecPolygon.clear();
    m_vecPolygon.push_back(polygon);

    FindBoudaries();
}

TGameEntity::~TGameEntity()
{

}

void TGameEntity::AddShape(TPolygon polygon)
{
    m_vecPolygon.push_back(polygon);
}

void TGameEntity::Draw()
{
    for(auto it = m_vecPolygon.begin(); it != m_vecPolygon.end(); it++)
        it->Draw(m_x, m_y);
}

void TGameEntity::FindBoudaries()
{
    float MaxW = m_vecPolygon[0].m_vec_vertex.m_vec_vertex[0].a, MaxH = m_vecPolygon[0].m_vec_vertex.m_vec_vertex[0].b;
    float MinW = m_vecPolygon[0].m_vec_vertex.m_vec_vertex[0].a, MinH = m_vecPolygon[0].m_vec_vertex.m_vec_vertex[0].b;
    for(auto it = m_vecPolygon.begin(); it != m_vecPolygon.end(); it++)
    {
        for(auto it2 = it->m_vec_vertex.m_vec_vertex.begin(); it2 != it->m_vec_vertex.m_vec_vertex.end(); it2++)
        {
            MaxW = max(MaxW, it2->a);
            MaxH = max(MaxH, it2->b);
            MinW = min(MinW, it2->a);
            MinH = min(MinH, it2->b);
        }
    }
    m_width = MaxW;
    m_height = MaxH;
    m_x += MinW;
    m_y += MinH;
}
