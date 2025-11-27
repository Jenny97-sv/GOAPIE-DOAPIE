#pragma once
#include "Component.h"
//#include "Sprite.h"
//#include "src/Engine/Graphics/Sprite/Sprite.h"
//#include "src/Engine/GameObject/GameObject.h"
//#include "src/Engine/Components/TransformComponent.h"

#include "Engine/Graphics/Sprite/Sprite.h"
#include "Engine/GameObject/GameObject.h"
#include "Engine/Components/TransformComponent.h"

class SpriteRendererComponent : public Component
{
public:
	Sprite sprite;

	void Init() override
	{
		transformComponent = owner->GetComponent<TransformComponent>();
	}

	void Update(const float dt) override
	{
		sprite.SetPosition(transformComponent->position);
		sprite.SetRotation(transformComponent->rotation);
		sprite.SetScale(transformComponent->scale);
	}

	void Render() override
	{
		sprite.Draw();
	}

	void SwitchColor(const Vector4f color)
	{
		sprite.SetColor(color);
	}

private: 
	TransformComponent* transformComponent = nullptr;
};