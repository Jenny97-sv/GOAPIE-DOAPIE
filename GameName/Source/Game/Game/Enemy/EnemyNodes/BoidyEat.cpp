#include "BoidyEat.h"
#include "GameObject.h"
#include "Random.h"
#include "SceneManager.h"
#include "Food.h"
#include "Boidy.h"
#include "BoidMath.h"

#include "TransformComponent.h"
#include "StatsComponent.h"
#include "FriendshipComponent.h"
#include "MovementComponent.h"
#include "HungerComponent.h"

bool BoidyEat::Start()
{
	food = SceneManager::GetInstance().GetActiveScene()->FindObjectOfType<Food>();
	if (!food)
		return false;

	hasArrived = false;

	host = blackboard->GetValue<Boidy*>("Host");
	if (!host)
		return false;

	transformComponent = host->GetComponent<TransformComponent>();
	statsComponent = host->GetComponent<StatsComponent>();
	friendshipComponent = host->GetComponent<FriendshipComponent>();
	movementComponent = host->GetComponent<MovementComponent>();
	hungerComponent = host->GetComponent<HungerComponent>();

	movementComponent->SetMode(eMovementMode::eDirect);
	movementComponent->SetDeceleration(1000);

	Vector2f position = transformComponent->position;

	TransformComponent* foodTrans = food->GetComponent<TransformComponent>();
	Vector2f foodPos = foodTrans->position;

	direction = (foodPos - position).GetNormalized();
	//distance = (food->WorldPosition() - host->WorldPosition()).Length();
	distance = (foodPos - position).Length();


	startPosition = position;

	endPosition = startPosition + direction * distance;

	startSpeed = blackboard->GetValue<float>("Speed");
	speed = startSpeed;
	if (speed == 0)
		return false;
	float normTasty = food->tastiness / food->maxTasty;
	float normFriendship = friendshipComponent->GetPercent();
	//speedMultiplier = 0.8f + 0.4f * normFriendship + 0.3f * normTasty;
	//speedMultiplier = std::clamp(speedMultiplier, 0.5f, 2.5f);
	//speed *= speedMultiplier;

	float h = 1.0f - hungerComponent->HungerPercent(); // hungry = 1, full = 0

	speedMultiplier =
		0.5f +
		1.2f * normTasty * h +   // hunger amplifies tasty
		0.8f * normFriendship;   // friendship boosts response

	speedMultiplier = std::clamp(speedMultiplier, 0.5f, 2.5f);
	speed *= speedMultiplier;

	std::cout << speedMultiplier << std::endl;

	timer = 0;
	maxTimer = 10;

	attackTimer = 0;
	maxAttackTimer = 1;

	return true;
}

eNodeStatus BoidyEat::Update(const float deltaTime)
{
	timer += deltaTime;
	t = timer / maxTimer;

	food = SceneManager::GetInstance().GetActiveScene()->FindObjectOfType<Food>();
	if (!food)
	{
		return eNodeStatus::eFailure; // Goes to interrupt
	}

	if (!hasArrived) // First state
	{
		Vector2f pos = transformComponent->position;
		distance = (food->GetComponent<TransformComponent>()->position - pos).Length();
		if (distance <= closeEnough)
			hasArrived = true;

		movementComponent->Move(direction, speed, deltaTime);

		//Vector2f toTarget = food->GetComponent<TransformComponent>()->position - pos;
		//float dist = toTarget.Length();

		//Vector2f dir = toTarget.GetNormalized();

		//float endRot = std::atan2(dir.y, dir.x) + (BoidMath::Pi / 2.0f);
		//transformComponent->rotation = endRot;
	}
	else
	{
		attackTimer += deltaTime;
		if (attackTimer >= maxAttackTimer)
		{
			Stats& foodStats = food->GetStatsComponent()->stats; 

			statsComponent->stats.AttackTime(foodStats);
			if (foodStats.HP <= 0)
				food->OnEaten(host);
			attackTimer = 0;
		}
	}

	if (t >= 1)
		return eNodeStatus::eSuccess;
	else
		return eNodeStatus::eRunning;
}

void BoidyEat::End()
{
	timer = 0;
	attackTimer = 0;
	blackboard->SetValue<float>("Speed", startSpeed);
}

void BoidyEat::Interrupt()
{
	timer = 0;
	attackTimer = 0;
	blackboard->SetValue<float>("Speed", startSpeed);
}
