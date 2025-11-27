#include "BoidyFlock.h"
#include "Boidy.h"
#include "Random.h"
#include "BoidMath.h"

#include "TransformComponent.h"
#include "StatsComponent.h"
#include "MovementComponent.h"

bool BoidyFlock::Start()
{
	//boidy = dynamic_cast<Boidy*>(owner);
	boidy = blackboard->GetValue<Boidy*>("Host");
	if (!boidy)
		return false;
	timer = 0;
	maxTimer = 2;
	statsComponent = boidy->GetComponent<StatsComponent>();
	transformComponent = boidy->GetComponent<TransformComponent>();
	movementComponent = boidy->GetComponent<MovementComponent>();

	movementComponent->SetDeceleration(100);
	movementComponent->SetMaxAcceleration(300);
	movementComponent->SetMaxSpeed(400);
	movementComponent->SetMode(eMovementMode::eAccelerated);
	return true;
}

eNodeStatus BoidyFlock::Update(const float deltaTime)
{
	timer += deltaTime;
	t = timer / maxTimer;

	if (boidy->flockSteering != Vector2f(0.0f, 0.0f))
	{
		movementComponent->Move(boidy->flockSteering, deltaTime);
		endRotation = std::atan2(boidy->flockSteering.y, boidy->flockSteering.x) + (BoidMath::Pi / 2.0f);
	}
	else
	{
		if (!noDirection)
		{
			direction.x = Random::GetRandomNumber(-1.f, 1.f), direction.y = Random::GetRandomNumber(-1.f, 1.f);
			noDirection = true;
		}
		endRotation = std::atan2(direction.y, direction.x) + (BoidMath::Pi / 2.0f);
		//boidy->WorldPosition() += direction * deltaTime * boidy->speed;
		transformComponent->position += direction * deltaTime * statsComponent->stats.Speed;
		movementComponent->Move(direction.GetNormalized(), deltaTime);
	}

	//boidy->WorldRotation() = BoidMath::Lerp(boidy->WorldRotation(), endRotation, t);
	transformComponent->rotation = BoidMath::Lerp(transformComponent->rotation, endRotation, t);

	if (t >= 1)
		return eNodeStatus::eSuccess;
	else
		return eNodeStatus::eRunning;
}

void BoidyFlock::End()
{
}

void BoidyFlock::Interrupt()
{
	// Optional: reset steering or stop movement
}
