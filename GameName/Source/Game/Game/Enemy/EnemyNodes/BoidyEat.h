#pragma once
#include "ActionNode.h"

class Boidy;
class Food;
class StatsComponent;
class TransformComponent;
class FriendshipComponent;
class MovementComponent;
class HungerComponent;
class BoidyEat : public Action
{
public:
	BoidyEat(Blackboard* owner) : Action(owner) {}

	bool Start() override;
	eNodeStatus Update(const float deltaTime) override;
	void End() override;
	void Interrupt() override;

private:
	Boidy* host = nullptr;
	Food* food = nullptr;
	StatsComponent* statsComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	FriendshipComponent* friendshipComponent = nullptr;
	MovementComponent* movementComponent = nullptr;
	HungerComponent* hungerComponent = nullptr;
	Vector2f startPosition = { 0,0 };
	Vector2f endPosition = { 0,0 };
	Vector2f direction = { 0,0 };
	float distance = 0;
	float closeEnough = 50;
	float attackTimer = 0;
	float maxAttackTimer = 1;
	float speed = 100;
	float startSpeed = 100;
	float speedMultiplier = 1;
	bool hasArrived = false;
};