#include "Game.h"
#include <iostream>

namespace game
{
    TGame::TGame()
    {
        m_vecCollisions.clear();
        m_msaa = true;
        m_collision_flag = false;
        m_collsion_depth = 0;
        m_border_bottom_last_connection_point = 90;
        m_border_top_last_connection_point = 90;
        m_vecBorderMeshes.resize(4);
    }

    TGame::~TGame()
    {

    }

    void TGame::Init(int window_width, int window_height)
    {
        m_window_width = window_width;
        m_window_height = window_height;

        TPolygon Triangle(255, 255, 255, 100);
        Triangle.addVertex(0, 50);
        Triangle.addVertex(50, 50);
        Triangle.addVertex(25, 0);
        AddEntity(TGameEntity("Player", "sprite_ship", Triangle, vec2(10, window_height / 2 - 25), vec2(10, 10), -1, 0));

        TPolygon Square(255, 0, 0, 100);
        Square.addVertex(0, 50);
        Square.addVertex(0, 0);
        Square.addVertex(50, 0);
        Square.addVertex(50, 50);
        AddEntity(TGameEntity("Enemy", "sprite_enemy_1", Square, vec2(500, 200), vec2(10, 10), -1, 0));

        bullet_shape.addVertex(0, 5);
        bullet_shape.addVertex(50, 5);
        bullet_shape.addVertex(50, 0);
        bullet_shape.addVertex(0, 0);
        bullet_shape.m_color.r = 255;
        bullet_shape.m_color.g = 0;
        bullet_shape.m_color.b = 0;
        bullet_shape.m_color.a = 100;

        /*
        bullet_shape.addVertex(0, 0);
        bullet_shape.addVertex(0, 50);
        bullet_shape.addVertex(5, 50);
        bullet_shape.addVertex(5, 0);
        bullet_shape.m_color.r = 255;
        bullet_shape.m_color.g = 0;
        bullet_shape.m_color.b = 0;
        bullet_shape.m_color.a = 100;
        */
        /*
        m_test_poly.m_color = color{255, 255, 255, 100};
        m_test_poly.addVertex(0, 0);
        m_test_poly.addVertex(25, 50);
        m_test_poly.addVertex(100, 90);
        m_test_poly.addVertex(50, 200);
        m_test_poly.addVertex(30, 200);
        m_test_poly.addVertex(30, 100);
        */

        m_vecBorderMeshes[0] = GenerateBorderMesh(true);
        m_vecBorderMeshes[1] = GenerateBorderMesh(true);
        m_vecBorderMeshes[2] = GenerateBorderMesh(false);
        m_vecBorderMeshes[3] = GenerateBorderMesh(false);
    }

    TPolygon TGame::GenerateBorderMesh(bool position_top)
    {
        TPolygon temp_polygon(255, 255, 255, 100);
        srand(GetTime());
        if(position_top)
        {
            temp_polygon.addVertex(0, BORDER_MESH_BOUND_Y);
            temp_polygon.addVertex(0, 0);
            temp_polygon.addVertex(m_window_width, 0);
            temp_polygon.addVertex(m_window_width, BORDER_MESH_BOUND_Y);

            srand(GetTime());
            float step = m_window_width / BORDER_MESH_GENERATION_STEPS;
            for(int i = 1; i < BORDER_MESH_GENERATION_STEPS; i++)
                temp_polygon.addVertex(m_window_width - (step * i), (rand() % BORDER_MESH_BOUND_Y) * 2 + 10);
        }
        else
        {
            temp_polygon.addVertex(0, BORDER_MESH_BOUND_Y * 2 + 20);
            temp_polygon.addVertex(0, 0);
            srand(GetTime() / 2);
            float step = m_window_width / BORDER_MESH_GENERATION_STEPS;
            for(int i = 1; i < BORDER_MESH_GENERATION_STEPS; i++)
                temp_polygon.addVertex((step * i), (rand() % BORDER_MESH_BOUND_Y) * 2 + 10);

            temp_polygon.addVertex(m_window_width, 0);
            temp_polygon.addVertex(m_window_width, BORDER_MESH_BOUND_Y * 2 + 20);

            temp_polygon.PrintVerticesOut();
        }
        return temp_polygon;
    }

    void TGame::AddEntity(TGameEntity entity)
    {
        m_vecEntities.push_back(entity);
    }

    bool TGame::FindForwardCollisionMinkowski(int entity_1, int entity_2, float deltaTime, float &penetration_depth)
    {
        TPolygon tempE1 = m_vecEntities[entity_1].m_vecPolygon[0]; //First entity
        TPolygon tempE2 = m_vecEntities[entity_2].m_vecPolygon[0]; //Second entity

        float X0_t = m_vecEntities[entity_1].m_x + m_vecEntities[entity_1].m_velocity_x * deltaTime;
        float Y0_t = m_vecEntities[entity_1].m_y + m_vecEntities[entity_1].m_velocity_y * deltaTime;

        float e2_X0_t = m_vecEntities[entity_2].m_x + m_vecEntities[entity_2].m_velocity_x * deltaTime;
        float e2_Y0_t = m_vecEntities[entity_2].m_y + m_vecEntities[entity_2].m_velocity_y * deltaTime;

        for(auto it = tempE1.m_vec_vertex_out.m_vec_vertex.begin(); it != tempE1.m_vec_vertex_out.m_vec_vertex.end(); it++)
        {
            it->a += X0_t;
            it->b += Y0_t;
        }

        for(auto it = tempE2.m_vec_vertex_out.m_vec_vertex.begin(); it != tempE2.m_vec_vertex_out.m_vec_vertex.end(); it++)
        {
            it->a += e2_X0_t;
            it->b += e2_Y0_t;
        }

        return tempE1.FindForwardCollsionMinkowski(tempE2, penetration_depth);
    }

    bool TGame::FindBorderCollisionMinkowski(int border, float deltaTime, float &penetration_depth)
    {
        TPolygon tempE1 = m_vecEntities[0].m_vecPolygon[0]; //Player
        TPolygon tempE2 = m_vecBorderMeshes[border]; //Border

        float X0_t = m_vecEntities[0].m_x + m_vecEntities[0].m_velocity_x * deltaTime;
        float Y0_t = m_vecEntities[0].m_y + m_vecEntities[0].m_velocity_y * deltaTime;

        float e2_X0_t;
        float e2_Y0_t;

        if(border == 0 || border == 1)
        {
            e2_X0_t = 0;
            e2_Y0_t = 0;
        }
        if(border == 2 || border == 3)
        {
            e2_X0_t = 0;
            e2_Y0_t = m_window_height - 180;
        }

        for(auto it = tempE1.m_vec_vertex_out.m_vec_vertex.begin(); it != tempE1.m_vec_vertex_out.m_vec_vertex.end(); it++)
        {
            it->a += X0_t;
            it->b += Y0_t;
        }

        for(auto it = tempE2.m_vec_vertex_out.m_vec_vertex.begin(); it != tempE2.m_vec_vertex_out.m_vec_vertex.end(); it++)
        {
            it->a += e2_X0_t;
            it->b += e2_Y0_t;
        }

        return tempE1.FindForwardCollsionMinkowski(tempE2, penetration_depth);
    }

    bool TGame::FindForwardCollisionPC(int entity_1, int entity_2, float deltaTime)
    {
        TPolygon tempE1 = m_vecEntities[entity_1].m_vecPolygon[0]; //First entity
        TPolygon tempE2 = m_vecEntities[entity_2].m_vecPolygon[0]; //Second entity

        float X0_t = m_vecEntities[entity_1].m_x + m_vecEntities[entity_1].m_velocity_x * deltaTime;
        float Y0_t = m_vecEntities[entity_1].m_y + m_vecEntities[entity_1].m_velocity_y * deltaTime;

        float e2_X0_t = m_vecEntities[entity_2].m_x + m_vecEntities[entity_2].m_velocity_x * deltaTime;
        float e2_Y0_t = m_vecEntities[entity_2].m_y + m_vecEntities[entity_2].m_velocity_y * deltaTime;

        for(auto it = tempE1.m_vec_vertex_out.m_vec_vertex.begin(); it != tempE1.m_vec_vertex_out.m_vec_vertex.end(); it++)
        {
            it->a += X0_t;
            it->b += Y0_t;
        }

        for(auto it = tempE2.m_vec_vertex_out.m_vec_vertex.begin(); it != tempE2.m_vec_vertex_out.m_vec_vertex.end(); it++)
        {
            it->a += e2_X0_t;
            it->b += e2_Y0_t;
        }

        return (tempE1.FindForwardCollsionPC(tempE2) || tempE2.FindForwardCollsionPC(tempE1));
    }

    bool TGame::FindBorderCollisionPC(int border, float deltaTime)
    {
        TPolygon tempE1 = m_vecEntities[0].m_vecPolygon[0]; //Player
        TPolygon tempE2 = m_vecBorderMeshes[border]; //Border

        float X0_t = m_vecEntities[0].m_x + m_vecEntities[0].m_velocity_x * deltaTime;
        float Y0_t = m_vecEntities[0].m_y + m_vecEntities[0].m_velocity_y * deltaTime;

        float e2_X0_t;
        float e2_Y0_t;

        if(border == 0 || border == 1)
        {
            e2_X0_t = 0;
            e2_Y0_t = 0;
        }
        if(border == 2 || border == 3)
        {
            e2_X0_t = 0;
            e2_Y0_t = m_window_height - 180;
        }

        for(auto it = tempE1.m_vec_vertex_out.m_vec_vertex.begin(); it != tempE1.m_vec_vertex_out.m_vec_vertex.end(); it++)
        {
            it->a += X0_t;
            it->b += Y0_t;
        }

        for(auto it = tempE2.m_vec_vertex_out.m_vec_vertex.begin(); it != tempE2.m_vec_vertex_out.m_vec_vertex.end(); it++)
        {
            it->a += e2_X0_t;
            it->b += e2_Y0_t;
        }

        return (tempE1.FindForwardCollsionPC(tempE2) || tempE2.FindForwardCollsionPC(tempE1));
    }

    void TGame::AddBullet()
    {
        TGameEntity bullet("Bullet", "sprite_bullet", bullet_shape, vec2(m_vecEntities[0].m_x + 51, m_vecEntities[0].m_y), vec2(0, 0), 0, 100);
        TGameEntity bullet2("Bullet", "sprite_bullet", bullet_shape, vec2(m_vecEntities[0].m_x + 51, m_vecEntities[0].m_y + m_vecEntities[0].m_height-5), vec2(0, 0), 0, 100);
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

    void TGame::EnemyAI()
    {
        for(auto it = m_vecEntities.begin(); it != m_vecEntities.end(); it++)
        {
            if(it->m_name == "Enemy")
            {
                it->m_shoot_timer++;
                if(it->m_shoot_timer > 100)
                {
                    //AddBullet();
                    it->m_shoot_timer = 0;
                }
            }

        }
    }

    void TGame::Update(float delta_time)
    {
        m_collision_flag = false;
        m_collsion_depth = 0;
        m_vecCollisions.clear();
        m_sleep_time++;

        EnemyAI();

        for(auto it = m_vecEntities.begin(); it != m_vecEntities.end(); it++)
        {
            it->m_shoot_delay++;
            if(it->m_life_time != -1)
                it->m_life_time++;

            if(((it->m_name == "Bullet") && (it->m_x > 2000)) || ((it->m_life_time != -1) && (it->m_life_time > it->m_max_life_time)))
            {
                m_vecEntities.erase(it);
                it--;
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

        if(m_vecEntities[0].m_x < 0)
        {
            m_vecEntities[0].m_x = 1;
            m_vecEntities[0].m_velocity_x = 0;
        }

        if(m_vecEntities[0].m_x + m_vecEntities[0].m_width > m_window_width)
        {
            m_vecEntities[0].m_x = m_window_width - m_vecEntities[0].m_width;
            m_vecEntities[0].m_velocity_x = 0;
        }

        ///Looking for border collision
        for(int i = 0; i < 4; i++)
        {
            if(FindBorderCollisionPC(i, delta_time))
            {
                collision temp_collision(0, 666+i, 0);
                m_vecCollisions.push_back(temp_collision);
                m_collision_flag = true;
            }
        }

        ///Looking for Collision
        for(unsigned int i = 0; i < m_vecEntities.size(); i++)
        {
            for(unsigned int j = i + 1; j < m_vecEntities.size(); j++)
            {
                if(FindForwardCollisionPC(i, j, delta_time))
                {
                    collision temp_collision(i, j, 0);
                    m_vecCollisions.push_back(temp_collision);
                    m_collision_flag = true;
                }
            }
        }

        ///Collision Processing (Borders)
        for(auto it = m_vecCollisions.begin(); it != m_vecCollisions.end(); it++)
        {
           if((it->eID_1 == 0) && (it->eID_2 >= 666))
           {
               m_vecEntities[0].m_velocity_x = -m_vecEntities[0].m_velocity_x;
               m_vecEntities[0].m_velocity_y = -m_vecEntities[0].m_velocity_y;
           }
        }

        ///Collision processing (entities)
        for(auto it = m_vecCollisions.begin(); it != m_vecCollisions.end(); it++)
        {
            if(it->eID_2 < 666 || it->eID_1 < 666)
            {
                m_vecEntities[0].m_velocity_x = -m_vecEntities[0].m_velocity_x;
                m_vecEntities[0].m_velocity_y = -m_vecEntities[0].m_velocity_y;
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
