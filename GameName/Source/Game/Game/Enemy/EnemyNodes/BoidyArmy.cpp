#include "BoidyArmy.h"
#include "GameObject.h"
#include "Random.h"
#include "BoidMath.h"
#include "Boidy.h"

#include "TransformComponent.h"
#include "StatsComponent.h"
#include "MovementComponent.h"

bool BoidyArmy::Start()
{
    timer = 0;
    maxTimer = 5;
    boidy = blackboard->GetValue<Boidy*>("Host");
    if (!boidy)
        return false;

    boidy->hasFormationSlot = true;
    transformComponent = boidy->GetComponent<TransformComponent>();
    statsComponent = boidy->GetComponent<StatsComponent>();
    movementComponent = boidy->GetComponent<MovementComponent>();

    //movementComponent->SetDeceleration(100);
    //movementComponent->SetMaxAcceleration(1);
    movementComponent->SetMode(eMovementMode::eDirect);

    return true;
}

eNodeStatus BoidyArmy::Update(const float deltaTime)
{
    if (!boidy || !boidy->hasFormationSlot) 
        return eNodeStatus::eFailure;

    timer += deltaTime;
    t = timer / maxTimer;

    Vector2f toTarget = boidy->formationTarget - transformComponent->position;
    float dist = toTarget.Length();

    if (dist > closeEnough)
    {
        Vector2f dir = toTarget.GetNormalized();
        movementComponent->Move(dir, deltaTime);
        //transformComponent->position += dir * statsComponent->stats.Speed * deltaTime;

        //float endRot = std::atan2(dir.y, dir.x) + (BoidMath::Pi / 2.0f);
        //transformComponent->rotation = endRot;
    }
    else
    {
        Vector2f dir = boidy->formationDirection;
        //movementComponent->Move(dir.GetNormalized(), deltaTime);
        transformComponent->position += dir * statsComponent->stats.Speed * deltaTime;

        float endRot = std::atan2(dir.y, dir.x) + (BoidMath::Pi / 2.0f);
        transformComponent->rotation = endRot;
    }

    lastPosition = transformComponent->position;

    if (t >= 1)
        return eNodeStatus::eSuccess;
    else
        return eNodeStatus::eRunning;
}


void BoidyArmy::End()
{
    timer = 0;
}

void BoidyArmy::Interrupt()
{
    timer = 0;
}
