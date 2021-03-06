#pragma once

#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <string>
#include <set>

#include "GameEntity.h"
#include "Structures.h"
#include "Utility.h"
#include "ETime.h"

#define DECIPATON 0.02
#define BULLET_VELOCITY 3000
#define BORDER_MESH_BOUND_Y 90
#define MIN_BORDER_MESH_DEPTH 10
#define BORDER_MESH_GENERATION_STEPS 10
#define DIRECTION_LEFT 1
#define DIRECTION_RIGHT 2

using std::vector;
using std::string;
using std::cout;

namespace game
{
    struct collision
    {
        public:
            int eID_1, eID_2;
            float ePenetrationDepth;

            collision();
            collision(int in1, int in2, float in3)
            {
                eID_1 = in1;
                eID_2 = in2;
                ePenetrationDepth = in3;
            }
    };

    struct particle
    {
        float m_current_life_time;
        float m_maximum_life_time;
        color m_color;
        vec2 m_pos;
        TPolygon m_shape;
    };

    class TGame
    {
        public:
            TGame();
            virtual ~TGame();

            void Input(bool* keys);
            void EnemyAI();
            void Update(float delta_time);
            void Init(int window_width, int window_height);
            void AddEntity(TGameEntity entity);
            void AddParticle(float x, float y, int for_random, int type);
            void Shoot(int shooter_entity_id, int direction, int ammount);
            void ClearCollisions();
            TPolygon GenerateBorderMesh(bool position_top);

            bool FindForwardCollisionMinkowski(int entity_1, int entity_2, float deltaTime, float &penetration_depth); ///Not really working yet
            bool FindBorderCollisionMinkowski(int border, float deltaTime, float &penetration_depth); ///Not really working yet

            bool FindForwardCollisionPC(int entity_1, int entity_2, float deltaTime);
            bool FindBorderCollisionPC(int border, float deltaTime);

            void SpawnPlayer();
            void SpawnEnemy(int for_random);
            void SpawnEnemies(int ammount);

            void Start();
            void GameOver();

            vector<TGameEntity> m_vecEntities;
            vector<collision> m_vecCollisions;
            vector<TPolygon> m_vecBorderMeshes;
            vector<particle> m_vecParticles;

            TPolygon m_test_poly;

            bool m_msaa;
            bool m_collision_flag;
            float m_collsion_depth;
            int m_sleep_time;

            int m_window_width;
            int m_window_height;

            float m_shaking_rating;
            float m_border_shift;

            float m_border_top_last_connection_point;
            float m_border_bottom_last_connection_point;

            bool m_game_over;
            bool m_enemy_spawn_state;
            bool m_is_running;

            int m_current_wave;
            int m_current_enemy_count;

            float m_score;
            float m_best_score;

            TPolygon bullet_shape;
            TPolygon bullet_shape_2;
            TPolygon player_shape;
            TPolygon enemy_shape;
            TPolygon particle_shape;
            TPolygon particle_shape_2;

            float m_player_particle_delay;

            float m_coef;
    };
}
