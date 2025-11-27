#pragma once
#include "ActionNode.h"

class BoidyWalk : public Action
{
public:
	BoidyWalk(Blackboard* owner) : Action(owner) {}

	bool Start() override;
	eNodeStatus Update(const float deltaTime) override;
	void End() override;
	void Interrupt() override;

private:

	Vector2f startPosition = { 0,0 };
	Vector2f endPosition = { 0,0 };
	Vector2f direction = { 0,0 };
	float distance = 0;
};