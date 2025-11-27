#include "PlayerSmile.h"
#include "GameObject.h"
#include "Player.h"
#include "BoidMath.h"
#include "PostMaster.h"

#include "TransformComponent.h"

bool PlayerSmile::Start(GameObject* host)
{
    player = dynamic_cast<Player*>(host);

    if (!player)
        return false;

    transformComponent = player->GetComponent<TransformComponent>();

    startRot = transformComponent->rotation;
    endRot = startRot + BoidMath::DegToRad(360.0f);  
    timer = 0.0f;
    maxTimer = 1.0f; 

    Message msg;
    msg.msg = eMessage::ePlayerSmiled;
    PostMaster::GetInstance().SendMsg(msg);

    return true;
}

bool PlayerSmile::Update(GameObject* host, const float deltaTime)
{
    timer += deltaTime;
    t = timer / maxTimer;

    if (t > 1.0f)
        t = 1.0f; 

    float newRott = BoidMath::Lerp<float>(startRot, endRot, t);
    //player->WorldRotation() = newRott;
    transformComponent->rotation = newRott;

    return t >= 1.0f;
}

void PlayerSmile::End(GameObject* host)
{
    //player->WorldRotation() = endRot;
    transformComponent->rotation = endRot;
}

void PlayerSmile::Interrupt(GameObject* host)
{
}
