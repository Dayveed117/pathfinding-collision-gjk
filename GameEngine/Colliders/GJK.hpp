#pragma once

#include "../GameEngine/Components/GameObject.hpp"
#include "../GameEngine/Components/Transform.hpp"
#include "ICollider.hpp"

#include <glm/glm.hpp>

using namespace glm;


struct Simplex
{

public:
    std::vector<vec3> points{};

    inline void add(vec3 support) { points.push_back(support); }

    vec3 getA()    { return points.back(); }
    vec3 getB()    { return points[points.size()- 2]; }
    vec3 getC()    { return points.front(); }
    void removeA() { points.erase(points.end()); }
    void removeB() { points.erase(points.end() - 1); }
    void removeC() { points.erase(points.begin()); }
    int size()     { return points.size(); }
    
    vec3 tripleProduct(vec3 v1, vec3 v2, vec3 v3)
    {
        return (v2 * glm::dot(v1, v3)) - (v1 * glm::dot(v2, v3));
    }

    bool containsOrigin(vec3* direction)
    {
        vec3 a = getA();
        auto ao = -a;

        // When simplex has 3 support points
        if (size() == 3)
        {
            vec3 b = getB();
            vec3 c = getC();

            auto ab = b - a;
            auto ac = c - a;

            // (AC x AB) x AB = AB(AB.dot(AC)) - AC(AB.dot(AB))
            vec3 abPerp = tripleProduct(ac, ab, ab);
            vec3 acPerp = tripleProduct(ab, ac, ac);

            if (dot(abPerp, ao) >= 0)
            {
                removeC();
                *direction = abPerp;
            }
            else if (dot(acPerp, ao) >= 0)
            {
                removeB();
                *direction = acPerp;
            }
            else
                return true;
        }
        else
        {
            // If simplex does not have 3 support points
            vec3 b = getB();
            auto ab = b - a;
            vec3 abPerp = tripleProduct(ab, ao, ab);
            *direction = abPerp;
        }

        return false;
    }
};


class GJK
{
public:
    static bool DetectCollision(ICollider* collider1, ICollider* collider2);
    static vec3 Support(ICollider* collider1, ICollider* collider2, vec3 direction);
};

