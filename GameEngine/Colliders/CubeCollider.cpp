#include "CubeCollider.hpp"

CubeCollider::CubeCollider(Transform* t)
{
    this->t = t;
}

glm::vec3 CubeCollider::FindFurthestPoint(const vec3 direction)
{
    // Initial point from polygon
    // Max initial point from polygon, projected in the world
    // Max distance from distance
    auto vertexi = Cube::GetVertices();
    auto model   = t->model;

    auto ipoint          = vec3(vertexi[0], vertexi[1], vertexi[2]);
    vec3 max_world_point = model * vec4(ipoint, 1.0f);
    float max_distance   = dot(direction, max_world_point);

    // x  y  z  nx ny nz
    // 0  1  2  3  4  5
    for (size_t i = 0; i < vertexi.size(); i += 6)
    {
        auto point       = vec3(vertexi[i], vertexi[i + 1], vertexi[i + 2]);
        vec3 world_point = model * vec4(ipoint, 1.0f);
        float distance   = dot(direction, world_point);

        if (distance > max_distance)
        {
            max_distance    = distance;
            max_world_point = world_point;
        }

    }

    return max_world_point;
}