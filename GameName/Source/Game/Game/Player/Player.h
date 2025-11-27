#pragma once
#include "GameObject.h"
#include "Observer.h"
#include "StateMachine.h"
#include "AnimationPlayer.h"

enum class AnimNames
{
	eIdle = 0,
	eWalking = 1,
	eCount
};

enum class PlayerStates
{
	eWalking = 0,
	eDash = 1,
	eSmile = 2,
	ePet = 3,
	eFeed = 4,
	eCount
};

class StateMachine;
class AnimationComponent;
class SpriteRendererComponent;
class TransformComponent;
class Player : public GameObject, public Observer
{
public:
	~Player() override;
	bool Init() override;
	void Start() override;
	void Update(const float deltaTime) override;
	void Render() override;

	void OnMessageSent(const Message msg) override;

	Vector2f GetDirection() { return direction; }
	void SetDirection(const Vector2f newDirection) { direction = newDirection; }
	void SetDirection(const float x, const float y) { direction = { x,y }; }
private:
	std::unique_ptr<StateMachine> stateMachine = nullptr;
	//std::unique_ptr<AnimationPlayer> animPlayer = nullptr;
	AnimationComponent* animationComponent = nullptr;
	SpriteRendererComponent* spriteRendererComponent = nullptr;
	TransformComponent* transformComponent = nullptr;


	Vector2f direction = { 0,0 };

	bool InitAnimations();
	bool InitStates();

	float speed = 100;
};