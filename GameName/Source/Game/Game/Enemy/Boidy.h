#pragma once
#include "Enemy.h"
#include "Observer.h"


class Food;
class SpriteRendererComponent;
class AnimationComponent;
class StatsComponent;
class TransformComponent;

class HungerComponent;
class FriendshipComponent;
class BoidyAIComponent;
class MovementComponent;


class Boidy : public Enemy, public Observer
{
public:
	~Boidy() override;
	bool Init() override;
	void Start() override;
	void Update(const float deltaTime) override;
	void Render() override;

	void OnMessageSent(const Message msg) override;

	//bool shouldEat = false;
private:
	TransformComponent* transformComponent;
	AnimationComponent* animationComponent;
	SpriteRendererComponent* spriteRendererComponent;
	StatsComponent* statsComponent;
	

	HungerComponent* hungerComponent;
	FriendshipComponent* friendshipComponent;
	BoidyAIComponent* aiComponent;
	MovementComponent* movementComponent;

	bool InitAnimations();
	bool InitStats();
	bool InitTransform();

	//Food* food = nullptr;

	//std::vector<Texture*> textures{};
};