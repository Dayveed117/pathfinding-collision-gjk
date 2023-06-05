#pragma once

#include "../GameEngine/Components/Transform.hpp"
#include "../GameEngine/Primitives/IPrimitive.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;

class ICollider
{

public:

    virtual glm::vec3 FindFurthestPoint(const glm::vec3 direction) = 0;
};
