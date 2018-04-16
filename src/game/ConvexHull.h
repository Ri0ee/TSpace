#pragma once

#include <vector>
#include <stack>
#include <algorithm>
#include "Structures.h"
#include <Utility.h>

#define CCW 1
#define CW 2
#define F 0

bool Convex(std::vector<vec2> vecIn, std::vector<vec2>& vecOut);

struct Hull{
    public:
        vec2 point;
        float angle;
        Hull()
        {
            point.a = 0;
            point.b = 0;
            angle = 0;
        }
        Hull(float in1, float in2, float in3)
        {
            point.a = in1;
            point.b = in2;
            angle = in3;
        }
        Hull(vec2 in1, float in2)
        {
            point = in1;
            angle = in2;
        }
};
