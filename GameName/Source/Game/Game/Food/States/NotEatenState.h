#pragma once
#include "StateMachine.h"

class StatsComponent;
class NotEatenState : public State
{
public:
	bool Start(GameObject* host) override;
	bool Update(GameObject* host, const float deltaTime) override;
	void End(GameObject* host) override;
	void Interrupt(GameObject* host) override;

private:
	StatsComponent* statsComponent = nullptr;
};