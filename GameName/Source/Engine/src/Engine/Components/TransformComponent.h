#pragma once
#include "Component.h"
#include "CommonUtilites/Vector/Vector.h"

class TransformComponent : public Component
{
public:
	Vector2f position;
	Vector2f scale;
	float rotation;
};