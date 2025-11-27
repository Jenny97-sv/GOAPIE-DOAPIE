#pragma once
#include "StateMachine.h"

class TransformComponent;

class PlayerDash : public State
{
public:
	bool Start(GameObject* host) override;
	bool Update(GameObject* host, const float deltaTime) override;
	void End(GameObject* host) override;
	void Interrupt(GameObject* host) override;

private:
	TransformComponent* transformComponent = nullptr;
	Vector2f startPosition = { 0,0 };
	Vector2f endPosition = { 0,0 };
	Vector2f dashDirection = { 0,0 };
	float distance = 0;
};