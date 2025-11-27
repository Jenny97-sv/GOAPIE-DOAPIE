#include "MovementComponent.h"
#include "TransformComponent.h"
#include "StatsComponent.h"
#include "GameObject.h"
#include "BoidMath.h"

void MovementComponent::Init()
{
    transformComponent = owner->GetComponent<TransformComponent>();
    statsComponent = owner->GetComponent<StatsComponent>();

    velocity = { 0.f, 0.f };
    acceleration = { 0.f, 0.f };

    maxSpeed = statsComponent ? statsComponent->stats.Speed : 200.f;
    maxAcceleration = 600.f;
    deceleration = 700.f;
    rotationSpeed = 10.f;

    movementMode = eMovementMode::eAccelerated; // default
}

void MovementComponent::Update(float dt)
{
    // Apply acceleration first
    velocity += acceleration * dt;

    float speed = velocity.Length();
    if (speed > maxSpeed)
        velocity = velocity.GetNormalized() * maxSpeed;

    // Smooth deceleration if not actively moving
    if (!isMoving && speed > 0.f)
    {
        float newSpeed = speed - deceleration * dt;
        if (newSpeed < 0.f) newSpeed = 0.f;
        velocity = velocity.GetNormalized() * newSpeed;
    }

    // Apply movement to transform
    transformComponent->position += velocity * dt;

    // Auto-rotate when moving
    if (velocity.LengthSqr() > 0.0001f)
    {
        float targetAngle = std::atan2(velocity.y, velocity.x) + (BoidMath::Pi / 2.0f);
        float current = transformComponent->rotation;
        float lerped = BoidMath::Lerp<float>(current, targetAngle, rotationSpeed * dt);
        transformComponent->rotation = lerped;
    }

    // Reset for next frame
    acceleration = { 0.f, 0.f };
    isMoving = false;
}


// ─────────────────────────────────────────────
//  Move() — version without speed override
// ─────────────────────────────────────────────
void MovementComponent::Move(const Vector2f direction, const float dt)
{
    if (direction.LengthSqr() < 0.0001f)
        return;

    isMoving = true;

    switch (movementMode)
    {
    case eMovementMode::eDirect:
    {
        // Immediately snap to full speed
        velocity = direction.GetNormalized() * maxSpeed;
        acceleration = { 0.f, 0.f };
        return;
    }

    case eMovementMode::eHybrid:
    {
        // Smooth but fast lerp toward target velocity
        Vector2f desiredVel = direction.GetNormalized() * maxSpeed;
        velocity = BoidMath::Lerp(velocity, desiredVel, dt * 8.0f);
        return;
    }

    case eMovementMode::eAccelerated:
    default:
    {
        // Your original acceleration-based movement
        Vector2f desiredAcc = direction.GetNormalized() * maxAcceleration;
        acceleration += desiredAcc;
        return;
    }
    }
}


// ─────────────────────────────────────────────
//  Move() — version WITH speed override
//  (used by BoidyEat for speedMultiplier)
// ─────────────────────────────────────────────
void MovementComponent::Move(const Vector2f direction, const float speed, const float dt)
{
    if (direction.LengthSqr() < 0.0001f)
        return;

    isMoving = true;

    float targetSpeed = speed; // use provided speed

    switch (movementMode)
    {
    case eMovementMode::eDirect:
    {
        velocity = direction.GetNormalized() * targetSpeed;
        acceleration = { 0.f, 0.f };
        return;
    }

    case eMovementMode::eHybrid:
    {
        Vector2f desiredVel = direction.GetNormalized() * targetSpeed;
        velocity = BoidMath::Lerp(velocity, desiredVel, dt * 8.0f);
        return;
    }

    case eMovementMode::eAccelerated:
    default:
    {
        // accelerate toward (speed * something)
        Vector2f desiredAcc = direction.GetNormalized() * (targetSpeed * maxAcceleration);
        acceleration += desiredAcc;
        return;
    }
    }
}
