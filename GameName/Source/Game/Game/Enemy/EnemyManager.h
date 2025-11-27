#pragma once
#include "Enemy.h"

enum class eFormationDirection
{
	eLeft,
	eUp,
	eRight,
	eDown,
	eCount
};

class TransformComponent;
class EnemyManager
{
public:
	const GameObject* GetPlayer() const { return player; }
	GameObject* GetPlayer() { return player; }
	void SetPlayer(GameObject* player);
	bool AddEnemy(Enemy* enemy);
	bool RemoveEnemy(Enemy* enemy);

	void AlertEnemies(const bool shouldAlert);
	void ShouldArmy(const bool shouldArmy);
	bool AssignFormationSlots();
	void CircleAroundPlayerPositions(const float deltaTime);
	void CircleAttack(const float deltaTime, const bool stopAttacking);
	void UpdateEnemies(const float deltaTime);
	void UpdateFormationSlots(Enemy*& boid, const int i, const float deltaTime);

	const bool GetAllInCirclePosition() const { return allInCirclePosition; }
	const bool GetAllStopCircleAttack() const { return allStopCircleAttack; }
	void SetAllStopCircleAttack(const bool shouldStopCircleAttack) { allStopCircleAttack = shouldStopCircleAttack; }
	//void StopCircleAttack();
	void ResetCircleAttackState();

	static EnemyManager& GetInstance()
	{
		static EnemyManager instance;
		return instance;
	}
private:
	void SetNewDirection(const Vector2f newDirection);
	void RotateBoids(const float degree);
	void UpdateFormationDirection(const float dt);
	std::vector<Enemy*> enemies = {};
	GameObject* player = nullptr;
	TransformComponent* playerTransform = nullptr;
	Vector2f newCenter = { 0,0 };
	Vector2f newDirection = { 0,0 };
	float distance = 100000;
	float distanceToPlayer = 100000;
	bool forceAlertEnemies = false;

	bool allInCirclePosition = false;
	bool allStopCircleAttack = false;
	int currentCircleAttacker = 0;
	float circleTimer = 0;
	float maxCircleTimer = 0.9f;

	eFormationDirection currentDirection = eFormationDirection::eCount;
	float formationTimer = 0;
	float maxFormationTimer = 4;
	float formationSpeed = 100;
	float spacing = 100;
	bool moveVertically = false;
};