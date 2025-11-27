#pragma once
#include "StateMachine.h"

class TransformComponent;
class PlayerSmile : public State
{
public:
	bool Start(GameObject* host) override;
	bool Update(GameObject* host, const float deltaTime) override;
	void End(GameObject* host) override;
	void Interrupt(GameObject* host) override;

private:
	TransformComponent* transformComponent = nullptr;
	float startRot = 1;
	float endRot = 1;
};