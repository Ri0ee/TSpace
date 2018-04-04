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
        TGameEntity(string entity_name, TPolygon polygon, vec2 origin, vec2 acceleration, string sprite_name);
        virtual ~TGameEntity();

        void AddShape(TPolygon polygon);
        void Draw();
        void FindBoudaries();

        string m_name;
        float m_x;
        float m_y;
        float m_width;
        float m_height;
        float m_velocity_y;
        float m_velocity_x;
        float m_acceleration_y;
        float m_acceleration_x;
        vector<TPolygon> m_vecPolygon;
};
