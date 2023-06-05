#include "GJK.hpp"
#include <iostream>

vec3 GJK::Support(ICollider* collider1, ICollider* collider2, vec3 direction)
{
    vec3 p1 = collider1->FindFurthestPoint(direction);
    vec3 p2 = collider2->FindFurthestPoint(-direction);

    return p1 - p2;
}

// Returns true if collider1 and collider2 overlap
// Return false
bool GJK::DetectCollision(ICollider* collider1, ICollider* collider2)
{
    /**
     * DetectCollision using GJK
     * 
     * Gerar um ponto aleatório d (direção).
     * Uma direção pode ser um ponto pois o seu vetor (é da origem até ao mesmo)
     * Achar o vértice mais distante na direção d (calcular dot entre d e todos os pontos do objecto)
     * 
     */

    float dx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    float dy = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    glm::vec3 direction = vec3(dx, dy, 0.0f);

    // Initialize simplex with first support point A
    auto simplex = Simplex();
    vec3 A = Support(collider1, collider2, direction);
    simplex.add(A);

    direction = -direction;

    // Tries on finding the collision
    size_t iterations = 50;
    for (size_t i = 0; i < iterations; i++)
    {
        simplex.add(Support(collider1, collider2, direction));
        
        // If second support does not go across origin, than no other point in minowski is.
        if (dot(simplex.getA(), direction) <= 0)
            return false;
        
        // If the current simplex encloses origin
        if (simplex.containsOrigin(&direction))
            return true;
    }

    return false;
}
