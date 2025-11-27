#include "BoidySpiral.h"
#include "GameObject.h"
#include "Random.h"
#include "Boidy.h"
#include "BoidMath.h"

#include "TransformComponent.h"
#include "StatsComponent.h"
#include "MovementComponent.h"

bool BoidySpiral::Start()
{
	timer = 0;
	maxTimer = 2;
	boidy = blackboard->GetValue<Boidy*>("Host");
    if (!boidy)
        return false;

    transformComponent = boidy->GetComponent<TransformComponent>();
    statsComponent = boidy->GetComponent<StatsComponent>();
    movementComponent = boidy->GetComponent<MovementComponent>();

    movementComponent->SetDeceleration(50);
    movementComponent->SetMaxAcceleration(100);
    movementComponent->SetMaxSpeed(500);
    movementComponent->SetMode(eMovementMode::eHybrid);
    return true;
}

eNodeStatus BoidySpiral::Update(const float deltaTime)
{
    timer += deltaTime;
    t = timer / maxTimer;

    //Vector2f radial = boidy->WorldPosition() - boidy->boidsCenter;
    Vector2f radial = transformComponent->position - boidy->boidsCenter;
    if (radial.Length() < 1.0f) radial = { 1,0 };

    Vector2f tangent = { -radial.y, radial.x };
    tangent.Normalize();
    radial.Normalize();

    // Lemniscate
    float curve = std::sin(t * BoidMath::Pi * 2.0f); // -1..1
    float spiralSpeed = /*curve * -*/1.0f; // Which direction they circle
    float radialBias = curve * -0.5f;
    
    // Sinewave
    //float radialBias = BoidMath::SineOscillate(timer, -0.5f, 0.5f, 0.25f);
    //float spiralSpeed = BoidMath::SineOscillate(timer, 0.8f, 1.2f, 0.5f);


    Vector2f spiralDir = tangent * spiralSpeed + radial * radialBias;
    spiralDir.Normalize();

    Vector2f separationDir = boidy->shouldSeperate ? boidy->seperationDirection : Vector2f{ 0,0 };

    float blendFactor = separationDir.LengthSqr() > 0.0001f ? 0.8f : 0.0f; // tune
    Vector2f finalDir = Vector2f::Lerp(spiralDir, separationDir, blendFactor);
    //if (finalDir.LengthSqr() > 0.0001f)
    finalDir.Normalize();

    //boidy->WorldPosition() += finalDir * boidy->speed * deltaTime;
    //transformComponent->position += finalDir * statsComponent->stats.Speed * deltaTime;
    movementComponent->Move(finalDir, deltaTime);

    endRot = std::atan2(finalDir.y, finalDir.x) + (BoidMath::Pi / 2.0f);
    //boidy->WorldRotation() = BoidMath::Lerp(boidy->WorldRotation(), endRot, 0.1f);
    transformComponent->rotation = BoidMath::Lerp(transformComponent->rotation, endRot, 0.1f);

    if (t >= 1)
        return eNodeStatus::eSuccess;
    else
        return eNodeStatus::eRunning;
}


void BoidySpiral::End()
{
}

void BoidySpiral::Interrupt()
{
}
