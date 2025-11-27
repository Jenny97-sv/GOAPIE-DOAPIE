#pragma once
#include "StateMachine.h"

class TransformComponent;
class PlayerMove : public State
{
public:
	bool Start(GameObject* host) override;
	bool Update(GameObject* host, const float deltaTime) override;
	void End(GameObject* host) override;
	void Interrupt(GameObject* host) override;

private:
	TransformComponent* transformComponent = nullptr;
	Vector2f previousPosition = { 0,0 };
	float speed = 300;
};