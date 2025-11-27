#pragma once
#include "ActionNode.h"

class BoidyIdle : public Action
{
public:
	BoidyIdle(Blackboard* owner) : Action(owner) {}
	bool Start() override;
	eNodeStatus Update(const float deltaTime) override;
	void End() override;
	void Interrupt() override;

private:
};