#pragma once

#include <vector>
#include <iostream>
#include <math.h>

#include "Polygon.h"
#include "Structures.h"
#include "Logger.h"
#include "Render.h"

using std::vector;
using std::string;

class TGameEntity
{
    public:
        TGameEntity(const string& entity_name, const string& sprite_name, TPolygon polygon, vec2 origin, vec2 acceleration, int life_time, int max_life_time);
        virtual ~TGameEntity();

        void AddShape(TPolygon polygon);
        void Draw();
        void FindBoudaries();

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

        int m_maximum_life;
        int m_current_life;
        int m_shoot_delay;
        int m_shoot_timer;

        vector<TPolygon> m_vecPolygon;
};
