#include "PlayerMovement.hpp"


PlayerMovement::PlayerMovement(GameObject* player, float xspeed, float yspeed)
{
    this->xspeed = xspeed;
    this->yspeed = yspeed;
    this->player = player;
    this->primitive = player->GetRenderer()->GetIPrimitive();
    this->scolor = this->primitive->GetColor();
}

void PlayerMovement::Update()
{
    auto t = player->GetTransform();
    
    if (Input::GetKey(KeyCode::W))
    {
        float yincr = Time::GetDeltaTime() * yspeed;
        t->Translate(vec3(0.f, yincr, 0.f));
    }
    
    if (Input::GetKey(KeyCode::A))
    {
        float xincr = Time::GetDeltaTime() * xspeed;
        t->Translate(vec3(-xincr, 0.f, 0.f));
    }

    if (Input::GetKey(KeyCode::S))
    {
        float yincr = Time::GetDeltaTime() * yspeed;
        t->Translate(vec3(0.f, -yincr, 0.f));
    }

    if (Input::GetKey(KeyCode::D))
    {
        float xincr = Time::GetDeltaTime() * xspeed;
        t->Translate(vec3(xincr, 0.f, 0.f));
    }

    primitive->UpdateColor(scolor);
}

void PlayerMovement::OnCollision(IPrimitive* other, string tag)
{
    primitive->UpdateColor(vec3(255.0f, 0.0f, 0.0f));
}
