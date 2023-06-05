#pragma once

#include "ICollider.hpp"
#include "../GameEngine/Primitives/Cube.hpp"

class CubeCollider : public ICollider
{
private:
    Transform* t;

public:
    CubeCollider(Transform* t);

    vec3 FindFurthestPoint(const vec3 direction);
};

