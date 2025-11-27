#pragma once
#include "StateMachine.h"

class SpriteRendererComponent;
class PlayerPet : public State
{
public:
	bool Start(GameObject* host) override;
	bool Update(GameObject* host, const float deltaTime) override;
	void End(GameObject* host) override;
	void Interrupt(GameObject* host) override;

private:
	SpriteRendererComponent* spriteRendererComponent = nullptr;
	Vector2f startSize = {};
	Vector2f endSize = {};
};