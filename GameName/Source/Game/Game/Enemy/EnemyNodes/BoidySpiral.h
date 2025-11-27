#pragma once
#include "ActionNode.h"

class Boidy;
class TransformComponent;
class StatsComponent;
class MovementComponent;
class BoidySpiral: public Action
{
public:
	BoidySpiral(Blackboard* owner) : Action(owner) {}

	bool Start() override;
	eNodeStatus Update(const float deltaTime) override;
	void End() override;
	void Interrupt() override;

private:
	TransformComponent* transformComponent = nullptr;
	StatsComponent* statsComponent = nullptr;
	MovementComponent* movementComponent = nullptr;
	Boidy* boidy = nullptr;

	Vector2f startPosition = { 0,0 };
	Vector2f endPosition = { 0,0 };
	Vector2f direction = { 0,0 };
	float distance = 0;

	float endRot = 0;
};