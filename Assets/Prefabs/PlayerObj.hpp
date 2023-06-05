/**
 * @file Player.hpp
 *
 * @brief Creates Player game objects
 *
 * @author Dário Santos
 * Contact: dariovfsantos@gmail.com
 */
#pragma once

#include <GL/glew.h>

#include "../GameEngine/Components/Transform.hpp"
#include "../GameEngine/Components/GameObject.hpp"
#include "../GameEngine/Components/Renderer.hpp"
#include "../GameEngine/Primitives/Square.hpp"
#include "../GameEngine/Primitives/Cube.hpp"
#include "../GameEngine/Colliders/ICollider.hpp"
#include "../GameEngine/Colliders/SquareCollider.hpp"
#include "../GameEngine/Colliders/CubeCollider.hpp"

 /*
 * Class: DeathZone
 * --------------------------------
 */
class PlayerObj
{
private:
    // This is private because we don't want Ball objects
    PlayerObj();
    ~PlayerObj();

public:
    static GameObject* AddPlayerObj(IPrimitive* ip, Transform* t, Material* material)
    {
        Renderer* r    = new Renderer(ip, material);
        ICollider* ic  = new SquareCollider(t);
        //GameObject* go = new GameObject(t, r, "Player");
        GameObject* go = new GameObject(t, r, ic, "Player");
        go->shader     = new OpaqueShader(r, "textures/na.png");

        return go;
    }
};
