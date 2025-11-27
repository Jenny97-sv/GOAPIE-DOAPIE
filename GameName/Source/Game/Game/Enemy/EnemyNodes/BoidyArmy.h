#pragma once
#include "ActionNode.h"

class Boidy;
class StatsComponent;
class MovementComponent;
class TransformComponent;
class BoidyArmy : public Action
{
public:
	BoidyArmy(Blackboard* owner) : Action(owner) {}

	bool Start() override;
	eNodeStatus Update(const float deltaTime) override;
	void End() override;
	void Interrupt() override;

private:
	Boidy* boidy = nullptr;
	StatsComponent* statsComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	MovementComponent* movementComponent = nullptr;

	Vector2f startPosition = { 0,0 };
	Vector2f endPosition = { 0,0 };
	Vector2f direction = { 0,0 };
	Vector2f lastPosition = { 0,0 };
	float distance = 0;
	float closeEnough = 10;
	bool hasReachedSlotPosition = false;
};