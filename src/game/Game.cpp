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
        m_shaking_rating = 0;
        m_border_shift = 0;
        m_game_over = false;
        m_enemy_spawn_state = false;
        m_is_running = false;
        m_current_wave = 1;
        m_current_enemy_count = 0;
        m_score = 0;
        m_best_score = 0;
    }

    TGame::~TGame()
    {

    }

    void TGame::Init(int window_width, int window_height)
    {
        m_window_width = window_width;
        m_window_height = window_height;

        bullet_shape.m_color = color{255, 0, 0, 100};
        bullet_shape.addVertex(0, 0);
        bullet_shape.addVertex(50, 0);
        bullet_shape.addVertex(50, 5);
        bullet_shape.addVertex(0, 5);

        bullet_shape_2.m_color = color{0, 255, 255, 100};
        bullet_shape_2.addVertex(0, 0);
        bullet_shape_2.addVertex(50, 0);
        bullet_shape_2.addVertex(50, 5);
        bullet_shape_2.addVertex(0, 5);

        player_shape.m_color = color{255, 255, 255, 100};
        player_shape.addVertex(0, 0);
        player_shape.addVertex(50, 25);
        player_shape.addVertex(0, 50);

        enemy_shape.m_color = color{255, 0, 0, 100};
        enemy_shape.addVertex(0, 0);
        enemy_shape.addVertex(50, 0);
        enemy_shape.addVertex(50, 50);
        enemy_shape.addVertex(0, 50);

//        SpawnPlayer();
//        SpawnEnemies(3);

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

        for(auto it = tempE1.m_vec_vertex.m_vec_vertex.begin(); it != tempE1.m_vec_vertex.m_vec_vertex.end(); it++)
        {
            it->a += X0_t;
            it->b += Y0_t;
        }

        for(auto it = tempE2.m_vec_vertex.m_vec_vertex.begin(); it != tempE2.m_vec_vertex.m_vec_vertex.end(); it++)
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
            e2_X0_t = 0 - m_border_shift;
            e2_Y0_t = 0;
        }
        if(border == 2 || border == 3)
        {
            e2_X0_t = 0 - m_border_shift;
            e2_Y0_t = m_window_height - 200;
        }

        for(auto it = tempE1.m_vec_vertex.m_vec_vertex.begin(); it != tempE1.m_vec_vertex.m_vec_vertex.end(); it++)
        {
            it->a += X0_t;
            it->b += Y0_t;
        }

        for(auto it = tempE2.m_vec_vertex.m_vec_vertex.begin(); it != tempE2.m_vec_vertex.m_vec_vertex.end(); it++)
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

        for(auto it = tempE1.m_vec_vertex.m_vec_vertex.begin(); it != tempE1.m_vec_vertex.m_vec_vertex.end(); it++)
        {
            it->a += X0_t;
            it->b += Y0_t;
        }

        for(auto it = tempE2.m_vec_vertex.m_vec_vertex.begin(); it != tempE2.m_vec_vertex.m_vec_vertex.end(); it++)
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

        if(border == 0)
        {
            e2_X0_t = 0 - m_border_shift;
            e2_Y0_t = 0;
        }
        if(border == 1)
        {
            e2_X0_t = m_window_width - m_border_shift;
            e2_Y0_t = 0;
        }

        if(border == 2)
        {
            e2_X0_t = 0 - m_border_shift;
            e2_Y0_t = m_window_height - 200;
        }

        if(border == 3)
        {
            e2_X0_t = m_window_width - m_border_shift;
            e2_Y0_t = m_window_height - 200;
        }

        for(auto it = tempE1.m_vec_vertex.m_vec_vertex.begin(); it != tempE1.m_vec_vertex.m_vec_vertex.end(); it++)
        {
            it->a += X0_t;
            it->b += Y0_t;
        }

        for(auto it = tempE2.m_vec_vertex.m_vec_vertex.begin(); it != tempE2.m_vec_vertex.m_vec_vertex.end(); it++)
        {
            it->a += e2_X0_t;
            it->b += e2_Y0_t;
        }

        return (tempE1.FindForwardCollsionPC(tempE2) || tempE2.FindForwardCollsionPC(tempE1));
    }

    void TGame::Shoot(int shooter_entity_id, int direction, int ammount)
    {
        float bullet_shift;
        if(ammount == 1)
            bullet_shift = m_vecEntities[shooter_entity_id].m_height / 2;
        if(ammount == 2)
            bullet_shift = m_vecEntities[shooter_entity_id].m_height;

        for(int i = 0; i < ammount; i++)
        {
            if(direction == DIRECTION_LEFT)
            {
                if(shooter_entity_id == 0)
                {
                    TGameEntity bullet(BULLET, "sprite_bullet", bullet_shape_2, vec2(m_vecEntities[shooter_entity_id].m_x - 30, m_vecEntities[shooter_entity_id].m_y + bullet_shift * (i + 1)), vec2(0, 0), 0, 100);
                    bullet.m_velocity_x = -BULLET_VELOCITY;
                    bullet.m_bullet_owner_id = shooter_entity_id;
                    AddEntity(bullet);
                }
                else
                {
                    TGameEntity bullet(BULLET, "sprite_bullet", bullet_shape, vec2(m_vecEntities[shooter_entity_id].m_x - 30, m_vecEntities[shooter_entity_id].m_y + bullet_shift * (i + 1)), vec2(0, 0), 0, 100);
                    bullet.m_velocity_x = -BULLET_VELOCITY;
                    bullet.m_bullet_owner_id = shooter_entity_id;
                    AddEntity(bullet);
                }
            }

            if(direction == DIRECTION_RIGHT)
            {
                if(shooter_entity_id == 0)
                {
                    TGameEntity bullet(BULLET, "sprite_bullet", bullet_shape_2, vec2(m_vecEntities[shooter_entity_id].m_x + m_vecEntities[shooter_entity_id].m_width + 1, m_vecEntities[shooter_entity_id].m_y + bullet_shift * i), vec2(0, 0), 0, 100);
                    bullet.m_velocity_x = BULLET_VELOCITY;
                    bullet.m_bullet_owner_id = shooter_entity_id;
                    AddEntity(bullet);
                }
                else
                {
                    TGameEntity bullet(BULLET, "sprite_bullet", bullet_shape, vec2(m_vecEntities[shooter_entity_id].m_x + m_vecEntities[shooter_entity_id].m_width + 1, m_vecEntities[shooter_entity_id].m_y + bullet_shift * i), vec2(0, 0), 0, 100);
                    bullet.m_velocity_x = BULLET_VELOCITY;
                    bullet.m_bullet_owner_id = shooter_entity_id;
                    AddEntity(bullet);
                }
            }
        }
        m_shaking_rating = m_shaking_rating + 0.5;
    }

    void TGame::Input(bool* keys)
    {
        if(m_is_running)
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
                if(m_vecEntities[0].m_shoot_delay > 40)
                {
                    Shoot(0, DIRECTION_RIGHT, 2);
                    m_vecEntities[0].m_shoot_delay = 0;
                }
        }

        ///Additional keys
        if(keys[GLFW_KEY_M])
            if(m_sleep_time > 50)
            {
                m_msaa = !m_msaa;
                m_sleep_time = 0;
            }

        if(keys[GLFW_KEY_ENTER])
        {
            if(m_game_over)
            {
                m_game_over = false;
            }
        }
    }

    void TGame::GameOver()
    {
        m_game_over = true;
        m_is_running = false;
        m_vecEntities.clear();
        m_vecCollisions.clear();
        if(m_score > m_best_score)
            m_best_score = m_score;
        m_score = 0;
    }

    void TGame::SpawnPlayer()
    {
        AddEntity(TGameEntity(PLAYER, "sprite_ship", player_shape, vec2(10, m_window_height / 2 - 25), vec2(10, 10), -1, 0));
    }

    void TGame::SpawnEnemy(int for_random)
    {
        srand(GetTime() * for_random);
        AddEntity(TGameEntity(ENEMY, "sprite_enemy_1", enemy_shape, vec2(m_window_width + 60 + rand() % 300, m_window_height / 2 + (rand() % 400 - 100)), vec2(10, 10), -1, 0));
    }

    void TGame::SpawnEnemies(int ammount)
    {
        for(int i = 0; i < ammount; i++)
        {
            SpawnEnemy(i);
        }
    }

    void TGame::EnemyAI()
    {
        m_current_enemy_count = 0;
        for(int i = 0; i < m_vecEntities.size(); i++)
        {
            if(m_vecEntities[i].m_type == ENEMY)
            {
                m_vecEntities[i].m_shoot_timer++;
                if(m_vecEntities[i].m_shoot_timer > 100)
                {
                    if(m_vecEntities[i].m_x <= m_window_width + m_vecEntities[i].m_width)
                    {
                        Shoot(i, DIRECTION_LEFT, 1);
                        m_vecEntities[i].m_shoot_timer = 0;
                    }
                }

                if(!m_vecEntities[i].m_task.m_is_active ||
                   m_vecEntities[i].m_task.m_current_duration >= m_vecEntities[i].m_task.m_maximum_duration)
                {
                    srand(GetTime() * i);
                    if(rand() % 2 == 1)
                    {
                        m_vecEntities[i].m_task.m_type = TASK_MOVE;
                        m_vecEntities[i].m_task.m_maximum_duration = rand() % 100 + 10;
                        m_vecEntities[i].m_task.m_current_duration = 0;
                        m_vecEntities[i].m_task.m_ax = -(rand() % 6 + 1);
                        if(rand() % 2 == 1)
                        {
                            if(m_vecEntities[i].m_y < m_window_height - 200)
                                m_vecEntities[i].m_task.m_ay = rand() % 6 + 1;
                        }
                        else
                        {
                            if(m_vecEntities[i].m_y > 300)
                                m_vecEntities[i].m_task.m_ay = -(rand() % 6 + 1);
                        }
                    }
                    else
                    {
                        m_vecEntities[i].m_task.m_type = TASK_STAY;
                        m_vecEntities[i].m_task.m_maximum_duration = rand() % 100 + 10;
                        m_vecEntities[i].m_task.m_current_duration = 0;
                        m_vecEntities[i].m_task.m_ax = 0;
                        m_vecEntities[i].m_task.m_ay = 0;
                    }

                    m_vecEntities[i].m_task.m_is_active = true;
                }

                if(m_vecEntities[i].m_task.m_is_active)
                {
                    m_vecEntities[i].m_task.m_current_duration++;
                    if(m_vecEntities[i].m_task.m_current_duration == m_vecEntities[i].m_task.m_maximum_duration)
                        m_vecEntities[i].m_task.m_is_active = false;

                    if(m_vecEntities[i].m_task.m_type == TASK_MOVE)
                    {
                        m_vecEntities[i].m_velocity_x += m_vecEntities[i].m_task.m_ax;
                        m_vecEntities[i].m_velocity_y += m_vecEntities[i].m_task.m_ay;
                    }

                    if(m_vecEntities[i].m_task.m_type == TASK_STAY)
                    {
                        m_vecEntities[i].m_velocity_x = 0;
                        m_vecEntities[i].m_velocity_y = 0;
                    }
                }
                m_current_enemy_count++;
            }
        }
    }

    void TGame::Start()
    {
        m_vecEntities.clear();
        m_vecCollisions.clear();
        SpawnPlayer();
        SpawnEnemies(2);
        m_is_running = true;
        m_game_over = false;
        m_current_wave = 1;
        m_current_enemy_count = 3 + int(m_current_wave / 2);
    }

    void TGame::Update(float delta_time)
    {
        m_collision_flag = false;
        m_collsion_depth = 0;
        m_vecCollisions.clear();
        m_sleep_time++;

        if(m_is_running)
        {
            if(m_vecEntities[0].m_current_life <= 0)
            {
                GameOver();
                return;
            }

            EnemyAI();
            if(m_current_enemy_count == 0)
            {
                m_current_wave++;
                SpawnEnemies(3 + int(m_current_wave / 2));
            }

            for(auto it = m_vecEntities.begin(); it != m_vecEntities.end(); it++)
            {
                it->m_shoot_delay++;
                if(it->m_life_time != -1)
                    it->m_life_time++;

                if(
                   ((it->m_type == BULLET) && (it->m_x > 2000)) ||
                   ((it->m_type == BULLET) && (it->m_did_hit)) ||
                   ((it->m_type == ENEMY) && (it->m_x < 0 - it->m_width)) ||
                   ((it->m_life_time != -1) && (it->m_life_time > it->m_max_life_time)) ||
                   ((it->m_is_destroyed) || (it->m_current_life <= 0))
                  )
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

            int ecount = m_vecEntities.size();

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
               if((it->eID_1 >= 666) || (it->eID_2 >= 666))
               {
                   if(it->eID_1 >= 666)
                   {
                       m_vecEntities[it->eID_2].m_velocity_x = -m_vecEntities[it->eID_2].m_velocity_x;
                       m_vecEntities[it->eID_2].m_velocity_y = -m_vecEntities[it->eID_2].m_velocity_y;
                   }

                   if(it->eID_2 >= 666)
                   {
                       m_vecEntities[it->eID_1].m_velocity_x = -m_vecEntities[it->eID_1].m_velocity_x;
                       m_vecEntities[it->eID_1].m_velocity_y = -m_vecEntities[it->eID_1].m_velocity_y;
                   }
               }
            }

            ///Collision processing (entities)
            for(auto it = m_vecCollisions.begin(); it != m_vecCollisions.end(); it++)
            {
                if(it->eID_2 < 666 && it->eID_1 < 666)
                {
                    if(m_vecEntities[it->eID_1].m_type == BULLET)
                    {
                        if(m_vecEntities[it->eID_2].m_type != BULLET)
                        {
                            if(!m_vecEntities[it->eID_1].m_did_hit && m_vecEntities[it->eID_1].m_bullet_owner_id != it->eID_2)
                            {
                                m_vecEntities[it->eID_2].m_current_life -= 3;
                                m_vecEntities[it->eID_1].m_did_hit = true;
                                if(it->eID_2 != 0 && m_vecEntities[it->eID_1].m_bullet_owner_id == 0)
                                {
                                    m_score += 3;
                                    if(m_vecEntities[it->eID_2].m_current_life <= 0)
                                        m_score += 10;
                                }
                            }
                        }
                    }

                    if(m_vecEntities[it->eID_2].m_type == BULLET)
                    {
                        if(m_vecEntities[it->eID_1].m_type != BULLET)
                        {
                            if(!m_vecEntities[it->eID_2].m_did_hit && m_vecEntities[it->eID_2].m_bullet_owner_id != it->eID_1)
                            {
                                m_vecEntities[it->eID_1].m_current_life -= 5;
                                m_vecEntities[it->eID_2].m_did_hit = true;
                                if(it->eID_1 != 0 && m_vecEntities[it->eID_2].m_bullet_owner_id == 0)
                                {
                                    m_score += 3;
                                    if(m_vecEntities[it->eID_1].m_current_life <= 0)
                                        m_score += 10;
                                }
                            }
                        }
                    }

                    if(it->eID_2 == 0 || it->eID_1 == 0)
                    {
                        m_vecEntities[0].m_velocity_x = -m_vecEntities[0].m_velocity_x;
                        m_vecEntities[0].m_velocity_y = -m_vecEntities[0].m_velocity_y;
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
}
