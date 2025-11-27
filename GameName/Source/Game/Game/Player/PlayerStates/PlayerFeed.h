#pragma once
#include "StateMachine.h"

class Orange;
class TransformComponent;
class PlayerFeed : public State
{
public:
	bool Start(GameObject* host) override;
	bool Update(GameObject* host, const float deltaTime) override;
	void End(GameObject* host) override;
	void Interrupt(GameObject* host) override;

private:
	GameObject* food = nullptr;
	TransformComponent* transformComponent = nullptr;
	float forwardOffset = 100;
	int random = -1;
};