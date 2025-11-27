#pragma once
#include <unordered_map>
//#include "Sprite.h"
//#include <src/Engine/Graphics/Sprite/Sprite.h>
//#include "src/Engine/Graphics/Sprite/Sprite.h"
//#include "Engine/Graphics/Sprite/Sprite.h"
#include "Engine/Graphics/Sprite/Sprite.h"
#include <memory>

class GameObject;
class AnimationPlayer
{
public:
	void InitAnimation(int enumID, std::vector<std::unique_ptr<Sprite>> sprites);
	void DeleteAnimations();
	void Update(Sprite& sprite, const float deltaTime);
	void SwitchAnimation(const int enumID);

private:
	std::unordered_map<int, std::vector<std::unique_ptr<Sprite>>> animations; // int used for each gameobject's own enum later
	int currentAnimation = -1;
	int currentFrame = -1;
	float frameLength = 0.2f;
	float frameTimer = 0;

	Vector2f previousPosition = { 0,0 };
	float previousRotation = 0;
	Vector2f previousScale = { 0,0 };
	Vector4f previosColor = { 1,1,1,1 };
};