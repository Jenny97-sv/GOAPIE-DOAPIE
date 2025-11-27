#include "PlayerPet.h"
#include "GameObject.h"
#include "Player.h"
#include "BoidMath.h"
#include "PostMaster.h"

#include "SpriteRendererComponent.h"

bool PlayerPet::Start(GameObject* host)
{
    player = dynamic_cast<Player*>(host);

    if (!player)
        return false;

    spriteRendererComponent = player->GetComponent<SpriteRendererComponent>();

    timer = 0.0f;
    maxTimer = 1.0f; 

    startSize = spriteRendererComponent->sprite.GetScale();
    endSize = { startSize.x / 2, startSize.y / 2 }; // It does have a / operator for scalar...

    Message msg;
    msg.msg = eMessage::ePlayerPet;
    PostMaster::GetInstance().SendMsg(msg);

    return true;
}

bool PlayerPet::Update(GameObject* host, const float deltaTime)
{
    timer += deltaTime;
    t = timer / maxTimer;

    if (t < 0.5)
        spriteRendererComponent->sprite.GetScale() = BoidMath::Lerp(startSize, endSize, t);
    else
        spriteRendererComponent->sprite.GetScale() = BoidMath::Lerp(endSize, startSize, t);

    return t >= 1.0f;
}

void PlayerPet::End(GameObject* host)
{
    spriteRendererComponent->sprite.GetScale() = startSize;
}

void PlayerPet::Interrupt(GameObject* host)
{
}
