#include "PointCollision.h"

bool isIn(vec2 l1, vec2 l2, vec2 v)
{
    if(((v.a - l1.a)*(l2.b - l1.b) - (v.b - l1.b)*(l2.a - l1.a)) < 0)
        return true; ///Left

    return false; ///Right
}

bool PointCollisionTest(std::vector<vec2> vecIn, vec2 vertex)
{
    if(isIn(vecIn[vecIn.size() - 1], vecIn[0], vertex))
    {
        for(unsigned int i = 0; i < vecIn.size() - 1; i++)
            if(!isIn(vecIn[i], vecIn[i+1], vertex))
                return false;
    }
    else return false;

    return true;
}
