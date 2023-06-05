#pragma once

#include "ICollider.hpp"
#include "../GameEngine/Primitives/Square.hpp"

class SquareCollider : public ICollider
{
private:
    Transform* t;

public:
    SquareCollider(Transform* t);

    vec3 FindFurthestPoint(const vec3 direction);
};

