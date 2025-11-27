#pragma once
#include "GameObject.h"
#include <memory>

static constexpr float maxFoodStatSize = 10;

enum class EatenState
{
	NotEatenState = 0,
	HalfEatenState = 1,
	Count
};

class StateMachine;
class AnimationComponent;
class StatsComponent;
class SpriteRendererComponent;
class TransformComponent;
class Boidy;
class Food : public GameObject
{
public:
	// Tastiness affects how happy boids are afterwards with friendshipMeter
	void OnEaten(Boidy* boid);

	const float maxTasty = maxFoodStatSize;
	float tastiness = 5;

	const float maxHealthy = maxFoodStatSize;
	float healthiness = 5;

	StateMachine* GetStateMachine() { return stateMachine.get(); }
	//AnimationPlayer* GetAnimPlayer() { return animPlayer.get(); }
	AnimationComponent* GetAnimationComponent() { return animationComponent; }
	StatsComponent* GetStatsComponent() { return statsComponent; }

protected:
	std::unique_ptr<StateMachine> stateMachine = nullptr; // Not good that anyone can control these...
	//std::unique_ptr<AnimationPlayer> animPlayer = nullptr;
	AnimationComponent* animationComponent = nullptr;
	StatsComponent* statsComponent = nullptr;
	SpriteRendererComponent* spriteRendererComponent = nullptr;
	TransformComponent* transformComponent = nullptr;

};