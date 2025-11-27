#pragma once
#include "ActionNode.h"

class Boidy;
class StatsComponent;
class TransformComponent;
class MovementComponent;
class BoidyAttack : public Action
{
public:
	BoidyAttack(Blackboard* owner) : Action(owner) {}

	bool Start() override;
	eNodeStatus Update(const float deltaTime) override;
	void End() override;
	void Interrupt() override;

private:
	StatsComponent* statsComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	MovementComponent* movementComponent = nullptr;
	void CalculatePositions();
	void MoveTowardsPositions(const Vector2f pos, Vector2f& dir, const float deltaTime);
	Boidy* host = nullptr;
	Vector2f startPosition = { 0,0 };
	Vector2f endPosition = { 0,0 };
	Vector2f direction = { 0,0 };
	float distance = 0;
	float closeEnough = 100.f;
	float attackTimer = 0;
	float maxAttackTimer = 2;
	bool isInCircle = false;
};