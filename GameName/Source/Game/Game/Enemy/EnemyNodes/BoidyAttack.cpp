#include "BoidyAttack.h"
#include "GameObject.h"
#include "Random.h"
#include "Boidy.h"
#include "EnemyManager.h"
#include "BoidMath.h"

#include "TransformComponent.h"
#include "StatsComponent.h"
#include "MovementComponent.h"

bool BoidyAttack::Start()
{
	host = blackboard->GetValue<Boidy*>("Host");
	if (!host)
		return false;

	transformComponent = host->GetComponent<TransformComponent>();
	statsComponent = host->GetComponent<StatsComponent>();
	movementComponent = host->GetComponent<MovementComponent>();

	movementComponent->SetDeceleration(1000);
	movementComponent->SetMode(eMovementMode::eHybrid);

	maxTimer = 10;
	timer = 0;
	isInCircle = false;

	attackTimer = 0;
	maxAttackTimer = 0.5f;

	CalculatePositions();

	host->circleAttack = eCircleAttack::eFindCirclePosition;

	EnemyManager::GetInstance().ResetCircleAttackState();
	//host->timeToStopCircleAttack = false;
	//host->timeToCircleAttack = false;
	//host->hasFoundCirclePosition = false;


	return true;
}

eNodeStatus BoidyAttack::Update(const float deltaTime)
{
	if (!host)
		return eNodeStatus::eFailure;

	timer += deltaTime;
	t = timer / maxTimer;
	//Vector2f pos = host->WorldPosition();
	Vector2f pos = transformComponent->position;
	Vector2f dir;
	EnemyManager& eMan = EnemyManager::GetInstance();
	CalculatePositions();

	if (!isInCircle) // First state
	{
		MoveTowardsPositions(pos, dir, deltaTime);

		distance = (pos - startPosition).Length();
		if (distance <= closeEnough)
		{
			host->circleAttack = eCircleAttack::eInCirclePosition;
		}
		//else
		//{
		//	static int i = 0;
		//	std::cout << distance << " " << i << std::endl;
		//	i++;
		//}
		if (eMan.GetAllInCirclePosition())
		{
			isInCircle = true;
		}

		//std::cout << "First" << std::endl;
	}
	else // Second state
	{
		if (host->circleAttack != eCircleAttack::eAttackCircle && host->circleAttack != eCircleAttack::eStopAttackCircle)
		{
			MoveTowardsPositions(pos, dir, deltaTime);
			if (t >= 1)
				return eNodeStatus::eSuccess;
			else
				return eNodeStatus::eRunning;
		}

		attackTimer += deltaTime;
		dir = (endPosition - pos).GetNormalized();
		//transformComponent->position = Vector2f::Lerp(startPosition, endPosition, attackTimer / maxAttackTimer);
		movementComponent->Move(dir, deltaTime);

		distance = (pos - endPosition).Length();
		if (eMan.GetAllStopCircleAttack())
		{
			return eNodeStatus::eSuccess;
		}
		else if (distance <= closeEnough)
		{
			host->circleAttack = eCircleAttack::eStopAttackCircle;
		}
		//std::cout << "Second" << std::endl;
	}

	//float endRot = std::atan2(dir.y, dir.x) + (BoidMath::Pi / 2.0f);
	//transformComponent->rotation = endRot;

	if (t >= 1)
		return eNodeStatus::eSuccess;
	else
		return eNodeStatus::eRunning;
}

void BoidyAttack::End()
{
	if (host)
	{
		host->circleAttack = eCircleAttack::eCount;
		host->shouldCircleAroundPlayer = false;
		host->hasFormationSlot = true;
	}

	attackTimer = 0;
	timer = 0;
	isInCircle = false;
	//EnemyManager::GetInstance().SetAllStopCircleAttack(false);
}


void BoidyAttack::Interrupt()
{
	if (host)
	{
		host->circleAttack = eCircleAttack::eCount;
		host->shouldCircleAroundPlayer = false;
		host->hasFormationSlot = true;
	}

	attackTimer = 0;
	timer = 0;
	isInCircle = false;
	//EnemyManager::GetInstance().SetAllStopCircleAttack(false);
}


void BoidyAttack::CalculatePositions()
{
	//Vector2f playerPos = EnemyManager::GetInstance().GetPlayer()->WorldPosition();
	Vector2f playerPos = EnemyManager::GetInstance().GetPlayer()->GetComponent<TransformComponent>()->position; // Not good doing this every update...
	startPosition = host->formationTarget;
	Vector2f toPlayer = (playerPos - startPosition).GetNormalized();
	float distToPlayer = (playerPos - startPosition).Length();
	endPosition = playerPos + toPlayer * distToPlayer;
}

void BoidyAttack::MoveTowardsPositions(const Vector2f pos, Vector2f& dir, const float deltaTime)
{
	dir = (startPosition - pos).GetNormalized();
	movementComponent->Move(dir, deltaTime);
	//transformComponent->position = transformComponent->position + dir * statsComponent->stats.Speed * deltaTime;
}
