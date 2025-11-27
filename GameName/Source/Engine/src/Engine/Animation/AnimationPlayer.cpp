#include "AnimationPlayer.h"
//#include "GameObject.h"
//#include "src/Engine/GameObject/GameObject.h"
#include "Engine/GameObject/GameObject.h"

void AnimationPlayer::InitAnimation(int enumID, std::vector<std::unique_ptr<Sprite>> sprites)
{
	if (animations.contains(enumID))
	{
		std::cout << "Already have this ID : " << enumID << std::endl;
		return;
	}

	animations.insert({ enumID, std::move(sprites) });
}
void AnimationPlayer::DeleteAnimations()
{
	animations.clear();
}
void AnimationPlayer::Update(Sprite& sprite, const float deltaTime) // Reference, so I don't make a copy, is that cheaper?
{
	frameTimer += deltaTime;
	if (frameTimer >= frameLength)
	{
		frameTimer = 0;
		if (currentFrame < animations[currentAnimation].size() - 1)
		{
			currentFrame++;
		}
		else
		{
			currentFrame = 0;
		}
	}
	previousPosition = sprite.GetPosition();
	previousRotation = sprite.GetRotation();
	previousScale = sprite.GetScale();
	previosColor = sprite.GetColor();
	sprite = *animations[currentAnimation][currentFrame]; // So transforms still are accurate, even when sprites change
	sprite.SetPosition(previousPosition);
	sprite.SetRotation(previousRotation);
	sprite.SetScale(previousScale);
	sprite.SetColor(previosColor);

	sprite.Draw();
}

void AnimationPlayer::SwitchAnimation(const int enumID)
{
	currentAnimation = enumID;
	currentFrame = 0;
	frameTimer = 0;
}
