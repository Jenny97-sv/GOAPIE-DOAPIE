#pragma once
#include "Component.h"
//#include "src/Engine/Animation/AnimationPlayer.h"
#include "Engine/Animation/AnimationPlayer.h"

class AnimationComponent : public Component
{
public:
	AnimationPlayer animPlayer;
};