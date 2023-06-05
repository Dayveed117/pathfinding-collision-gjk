#pragma once

#include <GL/glew.h>

#include "../GameEngine/Components/Transform.hpp"
#include "../GameEngine/Components/GameObject.hpp"
#include "../GameEngine/Components/Renderer.hpp"
#include "../GameEngine/Primitives/Cube.hpp"
#include "../GameEngine/Shader/Shader.hpp"
#include "../GameEngine/Colliders/ICollider.hpp"
#include "../GameEngine/Colliders/SquareCollider.hpp"
#include "../Shaders/OpaqueShader.hpp"

class Wall
{
private:
    Wall();
    ~Wall();

public:
    static GameObject* AddWall(Transform* t, vec3 color, Material* material)
    {
        IPrimitive* ip = new Square(color);
        ICollider* ic  = new SquareCollider(t);
        Renderer* r    = new Renderer(ip, material);
        //GameObject* go = new GameObject(t, r, "Wall");
        GameObject* go = new GameObject(t, r, ic, "Wall");
        go->shader     = new OpaqueShader(r, "textures/container.jpg");

        return go;
    }
};
