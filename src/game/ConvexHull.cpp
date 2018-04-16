#include "ConvexHull.h"

vec2 p0; ///Used for sorting, declaring outside

float GetPolarAngle(vec2 p0, vec2 p1)
{
    if(p1.b != p0.b)
        return -((p1.a - p0.a) / (p1.b - p0.b));

    if(p1.a < p0.a)
        return -1000000;
    if(p1.a > p0.a)
        return 1000000;
    return 0;
}

float GetDistance(vec2 p0, vec2 pDest)
{
    vec2 xy = vec2(pDest.a - p0.a, pDest.b - p0.b);
    return (xy.a * xy.a) + (xy.b * xy.b);
}

int GetOrientation(vec2 p1, vec2 p2, vec2 p3)
{
    float angl = (p2.b - p1.b) * (p3.a - p2.a) - (p3.b - p2.b) * (p2.a - p1.a);
    if(angl == 0) return F; ///forward
    return (angl > 0)?CCW:CW; ///counter clock wise; clock wise
}

vec2 nextToTop(std::stack<vec2> &input_stack)
{
    vec2 top_element_1 = input_stack.top();
    input_stack.pop();

    vec2 top_element_2 = input_stack.top();
    input_stack.push(top_element_1);

    return top_element_2;
}

std::vector<vec2> Vector(std::stack<vec2> in1)
{
    std::vector<vec2> tVec;
    while(!in1.empty())
    {
        tVec.insert(tVec.end(), in1.top());
        in1.pop();
    }
    return tVec;
}

bool compare(vec2 p1, vec2 p2)
{
    Hull h1, h2;
    h1 = Hull(p1, GetPolarAngle(p0, p1));
    h2 = Hull(p2, GetPolarAngle(p0, p1));
    if(h1.angle == h2.angle)
        return (GetDistance(p0, p1) <= GetDistance(p0, p2));
    else
        return (h1.angle > h2.angle);
}

bool Convex(std::vector<vec2> vecIn, std::vector<vec2>& vecOut)
{
    auto minY = vecIn[0].b;
    auto m = 0;
    for(auto i = 1; i < vecIn.size(); i++)
    {
        auto y = vecIn[i].b;
        if((y > minY) || (minY == y && vecIn[i].a < vecIn[m].a))
        {
            m = i;
            minY = y;
        }
    }

    iter_swap(vecIn.begin(), vecIn.begin() + m);
    p0 = vecIn[0];
    std::sort(vecIn.begin() + 1, vecIn.end(), compare);

    ///Remove nearest collinear points
    for(auto i = 1; i < vecIn.size(); i++)
    {
        auto j = i;
        auto temp_a1 = GetPolarAngle(p0, vecIn[i]);
        while(j < vecIn.size() - 1)
        {
            j++;
            auto temp_a2 = GetPolarAngle(p0, vecIn[j]);
            if(temp_a1 == temp_a2)
            {
                auto temp_dist1 = GetDistance(p0, vecIn[i]);
                auto temp_dist2 = GetDistance(p0, vecIn[j]);
                if((temp_dist1 == temp_dist2) || (temp_dist1 < temp_dist2))
                {
                    vecIn.erase(vecIn.begin() + i);
                    j--;
                }
            }
        }
    }

    if(vecIn.size() < 3)
        return false;

    ///Building convex hull
    std::stack<vec2> S;
    S.push(vecIn[0]);
    S.push(vecIn[1]);
    S.push(vecIn[2]);

    for(auto i = 3; i < vecIn.size(); i++)
    {
        while(GetOrientation(nextToTop(S), S.top(), vecIn[i]) != CCW)
            S.pop();

        S.push(vecIn[i]);
    }

    vecOut = Vector(S);

    return true;
}
