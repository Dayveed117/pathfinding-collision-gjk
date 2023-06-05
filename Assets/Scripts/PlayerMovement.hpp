#pragma once

#include "../GameEngine/Components/Script.hpp"
#include "../GameEngine/Components/Transform.hpp"
#include "../GameEngine/Components/GameObject.hpp"
#include "../GameEngine/Components/Script.hpp"
#include "../GameEngine/Primitives/IPrimitive.hpp"
#include "../GameEngine/Input/Input.hpp"
#include "../GameEngine/Time/Time.hpp"
#include "../GameEngine/Shader/Material.hpp"

class PlayerMovement : public Script
{
private:
    float xspeed;
    float yspeed;
    vec3 scolor;
    IPrimitive* primitive;
    GameObject* player;

public:
    PlayerMovement(GameObject* player, float xspeed, float yspeed);

    void Update() override;
    void OnCollision(IPrimitive* other, string tag) override;
};

