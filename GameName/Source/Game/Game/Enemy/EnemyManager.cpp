#include "EnemyManager.h"
#include "BoidMath.h"
#include "Random.h"
#include "Boidy.h"

#include "TransformComponent.h"

void EnemyManager::SetPlayer(GameObject* daplayer)
{
	player = daplayer; // can't be called same name....
	playerTransform = player->GetComponent<TransformComponent>();
}

bool EnemyManager::AddEnemy(Enemy* enemy)
{
	if (std::find(enemies.begin(), enemies.end(), enemy) != enemies.end())
	{
		std::cout << "Enemy already exists in Enemy Manager!" << std::endl;
		return false;
	}

	enemies.push_back(enemy);
	return true;
}

bool EnemyManager::RemoveEnemy(Enemy* enemy)
{
	for (size_t i = enemies.size() - 1; i > 0; i--)
	{
		if (enemies[i] == enemy)
		{
			enemies.erase(enemies.end() - i); // Gameworld owns the pointers, and deletes them by being unique
			return true;
		}
	}

	std::cout << "Couldn't find enemy in Enemy Manager" << std::endl;
	return false;
}

void EnemyManager::AlertEnemies(const bool shouldAlert)
{
	forceAlertEnemies = shouldAlert;

	for (Enemy*& boid : enemies)
	{
		boid->isAlerted = shouldAlert;
	}
	//std::cout << "Alerted everyone!" << std::endl;
}

void EnemyManager::ShouldArmy(const bool shouldArmy) // Not the best way, good for testing though
{
	for (Enemy*& boid : enemies)
	{
		boid->hasFormationSlot = shouldArmy;
	}
}

bool EnemyManager::AssignFormationSlots()
{
	if (enemies.empty()) return false;

	// Example: grid formation centered around boidsCenter
	const int cols = static_cast<int>(std::ceil(std::sqrt(enemies.size())));
	const int rows = static_cast<int>(std::ceil(enemies.size() / static_cast<float>(cols)));

	//Vector2f formationDirection = enemies[0]->formationDirection;
	Vector2f formationDirection = { 0,-1 };

	Vector2f right = { formationDirection.y, -formationDirection.x }; // perpendicular
	Vector2f forward = formationDirection.GetNormalized();
	newCenter = static_cast<Vector2f>(BoidMath::resolution) * 0.5f;

	currentDirection = eFormationDirection::eRight;
	newDirection = { 1,0 };
	moveVertically = true; // Depends on newDirection, if it moves in x, it will next time move in y

	int i = 0;
	for (Enemy* boid : enemies)
	{
		int row = i / cols;
		int col = i % cols;

		Vector2f offset =
			(col - cols / 2.f) * right * boid->formationSpacing +
			(row - rows / 2.f) * forward * boid->formationSpacing;

		boid->formationTarget = newCenter + offset;
		std::cout << boid->formationTarget << std::endl;
		boid->formationDirection = forward;
		boid->hasFormationSlot = true;

		i++;
	}

	return true;
}

void EnemyManager::CircleAroundPlayerPositions(const float deltaTime)
{
	if (enemies.empty() || !player) return;

	Vector2f center = playerTransform->position;
	float baseRadius = 150.f;
	float radius = baseRadius + enemies.size() * 5.f;

	float angleStep = 2 * BoidMath::Pi / enemies.size();
	bool allInCircle = true;
	bool allStopAttack = false;
	int doneAttacking = 0;

	for (int i = 0; i < enemies.size(); i++)
	{
		float angle = i * angleStep;
		float x = center.x + std::cos(angle) * radius;
		float y = center.y + std::sin(angle) * radius;
		Enemy* enemy = enemies[i];

		enemy->formationTarget = { x, y };
		if (enemy->circleAttack != eCircleAttack::eInCirclePosition && enemy->circleAttack != eCircleAttack::eAttackCircle && enemy->circleAttack != eCircleAttack::eStopAttackCircle)
			allInCircle = false;
		if (enemy->circleAttack == eCircleAttack::eStopAttackCircle)
			doneAttacking++;
	}
	if (doneAttacking >= enemies.size())
		allStopAttack = true;
	allInCirclePosition = allInCircle;
	if (allInCirclePosition)
		CircleAttack(deltaTime, allStopAttack);
	else
		currentCircleAttacker = 0;
}

void EnemyManager::CircleAttack(const float deltaTime, const bool stopAttacking)
{
	circleTimer += deltaTime;
	if (circleTimer >= maxCircleTimer)
	{
		circleTimer = 0;
		if (currentCircleAttacker < enemies.size() - 1)
		{
			allStopCircleAttack = false;
			currentCircleAttacker++;
			//std::cout << currentCircleAttacker << std::endl;
		}

	}

	if (stopAttacking)
	{
		allStopCircleAttack = true;
		//std::cout << "True" << std::endl;
		currentCircleAttacker = 0;
		circleTimer = 0;
		//for (auto& boid : enemies)
		//{
		//	if (Boidy* bo = dynamic_cast<Boidy*>(boid))
		//	{
		//		bo->GetBehaviorTree()->Reset();
		//	}
		//}
		//allInCirclePosition = false;
		return;
	}

	enemies[currentCircleAttacker]->circleAttack = eCircleAttack::eAttackCircle;
}




void EnemyManager::UpdateEnemies(const float deltaTime)
{
	int i = 0;
	UpdateFormationDirection(deltaTime);

	for (Enemy*& boid : enemies)
	{
		Vector2f separation = { 0, 0 };
		Vector2f alignment = { 0, 0 };
		Vector2f cohesion = { 0, 0 };

		int separationCount = 0;
		int alignmentCount = 0;
		int cohesionCount = 0;

		boid->shouldSeperate = false;
		boid->shouldCohede = false;
		boid->isAlerted = false;

		TransformComponent* trans;
		TransformComponent* otherTrans;

		Vector2f position;
		Vector2f otherPos;

		for (Enemy*& other : enemies)
		{
			if (boid == other) continue;

			trans = boid->GetComponent<TransformComponent>();
			otherTrans = other->GetComponent<TransformComponent>();

			position = trans->position;
			otherPos = otherTrans->position;

			float rotation = trans->rotation;
			float otherRot = otherTrans->rotation;

			Vector2f diff = position - otherPos;
			float dist = diff.Length();

			// Separation
			if (dist < boid->seperationDistance && dist > 0.001f)
			{
				boid->shouldSeperate = true;
				float weight = 1.0f - (dist / boid->seperationDistance);
				Vector2f away = (position - otherPos); // Should be getnormalized but then seperation barely registers...
				separation += away * weight;
				separationCount++;
			}

			// Alignment
			if (dist < boid->alignmentDistance)
			{
				float weight = /*1.0f - */(dist / boid->alignmentDistance);

				Vector2f dir = Vector2f{ std::cos(otherRot), std::sin(otherRot) }.GetNormalized();
				alignment += dir * weight;
				alignmentCount++;
			}

			// Cohesion
			if (dist < boid->cohesionDistance)
			{
				float weight = /*1.0f - */(dist / boid->cohesionDistance);
				cohesion += otherPos * weight;
				boid->shouldCohede = true;
				cohesionCount++;
			}

			boid->boidsCenter += otherPos;
		}

		if (boid->shouldCircleAroundPlayer)
			CircleAroundPlayerPositions(deltaTime);
		else if (boid->hasFormationSlot)
			UpdateFormationSlots(boid, i, deltaTime);

		boid->boidsCenter /= static_cast<float>(enemies.size());

		if (separationCount > 0)
			separation /= (float)separationCount;

		if (alignmentCount > 0)
			alignment = (alignment / (float)alignmentCount) - position.GetNormalized();

		if (cohesionCount > 0)
			cohesion = (cohesion / (float)cohesionCount) - position.GetNormalized();

		boid->alignmentDirection = alignment;
		boid->seperationDirection = separation;

		float sepWeight = 2.0f;
		float aliWeight = 0.1f;
		float cohWeight = 0.5f;

		Vector2f steering = sepWeight * separation
			+ aliWeight * alignment
			+ cohWeight * cohesion;

		steering.Normalize();

		boid->flockSteering = steering;

		if (!forceAlertEnemies && player)
		{
			distanceToPlayer = (position - playerTransform->position).Length();
			if (distanceToPlayer < boid->alertedDistance)
			{
				boid->isAlerted = true;
				//std::cout << "Distance to player = " << distanceToPlayer << ", alertedDistance = " << boid->alertedDistance << std::endl;
			}
		}
		else if (forceAlertEnemies)
		{
			boid->isAlerted = true;
			//std::cout << "Forced alertion" << std::endl;
		}

		i++;
	}
}
void EnemyManager::UpdateFormationSlots(Enemy*& boid, int i, float dt)
{
	int cols = (int)ceil(sqrt(enemies.size()));
	int rows = (int)ceil(enemies.size() / float(cols));

	int row = i / cols;
	int col = i % cols;

	float halfCols = (cols - 1) * 0.5f;
	float halfRows = (rows - 1) * 0.5f;

	float padding = 20;

	// --- FIXED AXES (grid no longer rotates) ---
	const Vector2f right = { 1, 0 };
	const Vector2f forward = { 0, 1 };

	Vector2f offset =
		(col - halfCols) * right * (spacing - padding) +
		(row - halfRows) * forward * (spacing - padding);

	boid->formationTarget = newCenter + offset;
	boid->formationDirection = newDirection; // just movement
}


void EnemyManager::UpdateFormationDirection(const float dt)
{
	newCenter += newDirection * dt * formationSpeed;

	int cols = (int)ceil(sqrt(enemies.size()));
	int rows = (int)ceil(enemies.size() / float(cols));

	float halfWidth = (cols - 1) * 0.5f * spacing;
	float halfHeight = (rows - 1) * 0.5f * spacing;

	float left = newCenter.x - halfWidth;
	float right = newCenter.x + halfWidth;
	float top = newCenter.y - halfHeight;
	float bottom = newCenter.y + halfHeight;

	float maxX = BoidMath::resolutionFloat.x;
	float maxY = BoidMath::resolutionFloat.y;
	float minX = 0;
	float minY = 0;

	bool outOfBounds =
		left < minX || right > maxX || top < minY || bottom > maxY;

	if (!outOfBounds) return;

	//std::cout << "OUTSIDE!!!" << std::endl;

	// --- FIX #2: avoid double-triggers by clamping ---
	if (left < minX)   newCenter.x = minX + halfWidth;
	if (right > maxX)  newCenter.x = maxX - halfWidth;
	if (top < minY)    newCenter.y = minY + halfHeight;
	if (bottom > maxY) newCenter.y = maxY - halfHeight;

	// choose axis to switch
	float distLeft = abs(newCenter.x - minX);
	float distRight = abs(maxX - newCenter.x);
	float distTop = abs(newCenter.y - minY);
	float distBottom = abs(maxY - newCenter.y);

	if (moveVertically)
		newDirection = (distTop > distBottom) ? Vector2f{ 0,-1 } : Vector2f{ 0,1 };
	else
		newDirection = (distLeft > distRight) ? Vector2f{ -1,0 } : Vector2f{ 1,0 };

	moveVertically = !moveVertically;

}


//void EnemyManager::StopCircleAttack()
//{
//	allStopCircleAttack = true;
//	currentCircleAttacker = 0;
//	circleTimer = 0;
//}

void EnemyManager::ResetCircleAttackState()
{
	allStopCircleAttack = false;
	//std::cout << "False" << std::endl;
	currentCircleAttacker = 0;
	circleTimer = 0;

	// Reset each boid's attack state
	for (Enemy*& enemy : enemies)
	{
		enemy->circleAttack = eCircleAttack::eFindCirclePosition;
		enemy->shouldCircleAroundPlayer = true;
		enemy->hasFormationSlot = false;
	}
}



void EnemyManager::SetNewDirection(const Vector2f newDirection)
{
	for (Enemy*& boid : enemies)
	{
		boid->formationDirection = newDirection;
	}
}

void EnemyManager::RotateBoids(const float degree)
{
	for (Enemy*& boid : enemies)
	{
		//boid->WorldRotation() = BoidMath::DegToRad(degree);
		boid->GetComponent<TransformComponent>()->rotation = BoidMath::DegToRad(degree);
	}
}

