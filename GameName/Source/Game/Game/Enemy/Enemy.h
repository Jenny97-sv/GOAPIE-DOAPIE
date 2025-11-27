#pragma once
#include "GameObject.h"
#include "CommonUtilites/Vector/Vector.h"

enum eCircleAttack
{
	eFindCirclePosition,
	eInCirclePosition,
	eAttackCircle,
	eStopAttackCircle,
	eCount
};

class Enemy : public GameObject // Add more when knowing shit
{
public:
	//~Enemy() override {}
	bool isAlerted = false;
	bool shouldSeperate = false;
	bool shouldCohede = false;
	bool hasFormationSlot = false;
	bool hasTakenDamage = false;
	bool isLookingForPlayer = false;
	eCircleAttack circleAttack = eCount;
	bool shouldCircleAroundPlayer = false;
	//bool hasFoundCirclePosition = false;
	//bool timeToCircleAttack = false;
	//bool timeToStopCircleAttack = false;
	unsigned int formationSlot = 0;

	Vector2f alignmentDirection = { 0,0 };
	Vector2f seperationDirection = { 0,0 };
	Vector2f flockSteering = { 0,0 };
	Vector2f boidsCenter = { 0,0 };
	Vector2f formationDirection = { 0,0 };
	Vector2f formationTarget = { 0,0 };

	float alertedDistance = 10000;
	float seperationDistance = 10000;
	float cohesionDistance = 0.1f;
	float alignmentDistance = 0.1f;
	float formationSpacing = 100;
	float formationSpeed = 10;

	virtual void InitEnemy(const float alertDistance, const float seperateDistance, const float cohedeDistance, const float alignDistance)
	{
		alertedDistance = alertDistance, seperationDistance = seperateDistance, cohesionDistance = cohedeDistance, alignmentDistance = alignDistance;
	}
};