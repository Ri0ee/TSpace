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
#define BULLET_VELOCITY 5000
#define BORDER_MESH_BOUND_Y 90
#define MIN_BORDER_MESH_DEPTH 10
#define BORDER_MESH_GENERATION_STEPS 10

using std::vector;
using std::set;
using std::string;
using std::cout;

namespace game
{
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
            void AddBullet();
            void ClearCollisions();
            TPolygon GenerateBorderMesh(bool position_top);
            int FindForwardCollisionM(int eID1, int eID2, float deltaTime, float &penetrationDepth);
            TGameEntity& GetEntity(string entity_name);

            vector<TGameEntity> m_vecEntities;
            vector<collision> m_vecCollisions;
            vector<TPolygon> m_vecBorderMeshes;

            bool m_msaa;
            bool m_collision_flag;
            float m_collsion_depth;
            int m_sleep_time;
            int m_entity_count;

            int m_window_width;
            int m_window_height;

            float m_border_top_last_connection_point;
            float m_border_bottom_last_connection_point;

            TPolygon bullet_shape;
    };
}
