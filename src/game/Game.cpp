#include "Game.h"

namespace game
{
    TGame::TGame()
    {
        m_vecCollisions.clear();
        m_msaa = false;
        m_collision_flag = false;
        m_collsion_depth = 0;
        m_entity_count = 0;

        TPolygon Triangle(255, 255, 255, 100);
        Triangle.addVertex(0, 50);
        Triangle.addVertex(50, 50);
        Triangle.addVertex(25, 0);
        AddEntity(TGameEntity("Player", "sprite_ship", Triangle, vec2(10, 200), vec2(10, 10), -1, 0));

        bullet_shape.addVertex(0, 5);
        bullet_shape.addVertex(0, 0);
        bullet_shape.addVertex(50, 0);
        bullet_shape.addVertex(50, 5);
        bullet_shape.m_color.r = 255;
        bullet_shape.m_color.g = 0;
        bullet_shape.m_color.b = 0;
        bullet_shape.m_color.a = 100;
    }

    TGame::~TGame(){}

    void TGame::SetBorders(int window_width, int window_height)
    {
        m_window_width = window_width;
        m_window_height = window_height;
    }

    void TGame::AddEntity(TGameEntity entity)
    {
        m_vecEntities.push_back(entity);
        m_entity_count++;
    }

    TGameEntity& TGame::GetEntity(string entity_name)
    {
        for(unsigned int i = 0; i < m_vecEntities.size(); i++)
            if(m_vecEntities[i].m_name == entity_name)
                return m_vecEntities[i];
        return m_vecEntities[0];
    }

    int TGame::FindForwardCollisionM(int eID1, int eID2, float deltaTime, float &penetrationDepth)
    {
        TGameEntity e1 = m_vecEntities[eID1];
        TGameEntity e2 = m_vecEntities[eID2];

        TPolygon tempE1 = e1.m_vecPolygon[0];
        TPolygon tempE2 = e2.m_vecPolygon[0];

        float X0_t = e1.m_x + e1.m_velocity_x * deltaTime;
        float Y0_t = e1.m_y + e1.m_velocity_y * deltaTime;

        float e2_X0_t = e2.m_x + e2.m_velocity_x * deltaTime;
        float e2_Y0_t = e2.m_y + e2.m_velocity_y * deltaTime;

        for(unsigned int i = 0; i < tempE1.m_vec_vertex_out.m_vec_vertex.size(); i++)
        {
            tempE1.m_vec_vertex_out.m_vec_vertex[i].a += X0_t;
            tempE1.m_vec_vertex_out.m_vec_vertex[i].b += Y0_t;
        }

        for(unsigned int i = 0; i < tempE2.m_vec_vertex_out.m_vec_vertex.size(); i++)
        {
            tempE2.m_vec_vertex_out.m_vec_vertex[i].a += e2_X0_t;
            tempE2.m_vec_vertex_out.m_vec_vertex[i].b += e2_Y0_t;
        }

        bool ans = tempE1.m_vec_vertex_out.Intersect(tempE2.m_vec_vertex_out);
        if(ans) penetrationDepth = tempE1.m_vec_vertex_out.m_nearest_point_length;
        return ans;
    }

    void TGame::AddBullet()
    {
        TGameEntity bullet("Bullet", "sprite_bullet", bullet_shape, vec2(m_vecEntities[0].m_x, m_vecEntities[0].m_y), vec2(0, 0), 0, 100);
        TGameEntity bullet2("Bullet", "sprite_bullet", bullet_shape, vec2(m_vecEntities[0].m_x, m_vecEntities[0].m_y + m_vecEntities[0].m_height-5), vec2(0, 0), 0, 100);
        bullet.m_velocity_x = BULLET_VELOCITY;
        bullet2.m_velocity_x = BULLET_VELOCITY;
        AddEntity(bullet);
        AddEntity(bullet2);
    }

    void TGame::Input(bool* keys)
    {
        ///Vertical movement
        if(keys[GLFW_KEY_W])
            m_vecEntities[0].m_velocity_y -= m_vecEntities[0].m_acceleration_y;

        if(keys[GLFW_KEY_S])
            m_vecEntities[0].m_velocity_y += m_vecEntities[0].m_acceleration_y;

        if(keys[GLFW_KEY_UP])
            m_vecEntities[0].m_velocity_y -= m_vecEntities[0].m_acceleration_y;

        if(keys[GLFW_KEY_DOWN])
            m_vecEntities[0].m_velocity_y += m_vecEntities[0].m_acceleration_y;

        ///Horizontal movement
        if(keys[GLFW_KEY_D])
            m_vecEntities[0].m_velocity_x += m_vecEntities[0].m_acceleration_x;

        if(keys[GLFW_KEY_A])
            m_vecEntities[0].m_velocity_x -= m_vecEntities[0].m_acceleration_x;

        if(keys[GLFW_KEY_RIGHT])
            m_vecEntities[0].m_velocity_x += m_vecEntities[0].m_acceleration_x;

        if(keys[GLFW_KEY_LEFT])
            m_vecEntities[0].m_velocity_x -= m_vecEntities[0].m_acceleration_x;

        if(keys[GLFW_KEY_ENTER] || keys[GLFW_KEY_SPACE])
            if(m_vecEntities[0].m_shoot_delay > 20)
            {
                AddBullet();
                m_vecEntities[0].m_shoot_delay = 0;
            }

        ///Additional keys
        if(keys[GLFW_KEY_M])
            if(m_sleep_time > 50)
            {
                m_msaa = !m_msaa;
                m_sleep_time = 0;
            }
    }

    void TGame::Update(float delta_time)
    {
        m_collision_flag = false;
        m_collsion_depth = 0;
        m_vecCollisions.clear();
        m_sleep_time++;

        for(auto it = m_vecEntities.begin(); it != m_vecEntities.end(); it++)
        {
            it->m_shoot_delay++;
            if(it->m_life_time != -1)
                it->m_life_time++;

            if(((it->m_name == "Bullet") && (it->m_x > 2000)) ||
               ((it->m_life_time != -1) && (it->m_life_time > it->m_max_life_time)))
            {
                m_vecEntities.erase(it);
                it--;
                m_entity_count--;
            }
        }

        if(m_vecEntities[0].m_y < 0)
        {
            m_vecEntities[0].m_y = 1;
            m_vecEntities[0].m_velocity_y = 0;
        }

        if(m_vecEntities[0].m_y > m_window_height - m_vecEntities[0].m_height)
        {
            m_vecEntities[0].m_y = m_window_height - m_vecEntities[0].m_height - 1;
            m_vecEntities[0].m_velocity_y = 0;
        }


        ///Looking for Collision
        for(unsigned int i = 0; i < m_vecEntities.size(); i++)
        {
            for(unsigned int j = i + 1; j < m_vecEntities.size(); j++)
            {
                float tempPenetrationDepth = 0;
                if(FindForwardCollisionM(i, j, delta_time, tempPenetrationDepth))
                {
                    collision tempCollision(i, j, tempPenetrationDepth);
                    m_vecCollisions.push_back(tempCollision);
                    m_collision_flag = true;
                    m_collsion_depth = tempPenetrationDepth;
                }
            }
        }

        ///Position Processing
        for(auto ent = m_vecEntities.begin(); ent != m_vecEntities.end(); ent++)
        {
            ent->m_velocity_x = ent->m_velocity_x - ent->m_velocity_x * DECIPATON;
            ent->m_velocity_y = ent->m_velocity_y - ent->m_velocity_y * DECIPATON;

            ent->m_x += ent->m_velocity_x * delta_time;
            ent->m_y += ent->m_velocity_y * delta_time;
        }
    }
}
