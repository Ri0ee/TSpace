#pragma once

#include <vector>
#include <iostream>
#include <math.h>

#include "Polygon.h"
#include "Structures.h"
#include "Logger.h"
#include "Render.h"

#define PLAYER 0
#define ENEMY 1
#define BULLET 2

using std::vector;
using std::string;

class TGameEntity
{
    public:
        TGameEntity(int entity_type, const string& sprite_name, TPolygon polygon, vec2 origin, vec2 acceleration, int life_time, int max_life_time);
        virtual ~TGameEntity();

        void AddShape(TPolygon polygon);
        void Draw();
        void FindBoudaries();

        int m_type;
        string m_name;
        string m_sprite_name;
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_velocity_y;
        float m_velocity_x;
        float m_acceleration_y;
        float m_acceleration_x;
        int m_life_time;
        int m_max_life_time;
        bool m_static_speed;

        float m_maximum_life;
        float m_current_life;
        float m_maximum_ammo;
        float m_current_ammo;

        int m_shoot_delay;
        int m_shoot_timer;

        bool m_is_destroyed;
        bool m_did_hit;

        vector<TPolygon> m_vecPolygon;
};
