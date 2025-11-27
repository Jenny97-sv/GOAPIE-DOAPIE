#pragma once
#include "Component.h"
#include <memory>
#include <vector>
#include "CommonUtilites/Vector/Vector.h"

enum class eMovementMode
{
	eDirect,
	eAccelerated,
	eHybrid,
	eNone
};

class TransformComponent;
class StatsComponent;
class MovementComponent : public Component
{
public:
	void Init() override;
	void Update(const float deltaTime) override;

	void Move(const Vector2f direction, const float deltaTime);
	void Move(const Vector2f direction, const float speed, const float deltaTime);

	void SetMaxAcceleration(const float acceleration) { maxAcceleration = acceleration; }
	void SetMaxSpeed(const float speed) { maxSpeed = speed;}
	void SetDeceleration(const float dec) { deceleration = dec; }
	void SetMode(const eMovementMode mode) { movementMode = mode; }
private:
	TransformComponent* transformComponent = nullptr;
	StatsComponent* statsComponent = nullptr;

	Vector2f velocity{ 0,0 };
	Vector2f acceleration{ 0,0 };

	eMovementMode movementMode = eMovementMode::eNone;

	const float friction = 8;
	float deceleration = 0;
	float rotationSpeed = 0;
	float maxSpeed = 0;
	float maxAcceleration = 1;
	bool isMoving = false;
};