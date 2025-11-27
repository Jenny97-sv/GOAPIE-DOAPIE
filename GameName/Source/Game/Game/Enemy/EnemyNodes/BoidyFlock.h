#pragma once
#include "ActionNode.h"

class Boidy;
class StatsComponent;
class TransformComponent;
class MovementComponent;
class BoidyFlock : public Action
{
public:
	BoidyFlock(Blackboard* owner) : Action(owner) {}

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
	float endRotation = 0;
	float distance = 0;
	bool noDirection = false;
};