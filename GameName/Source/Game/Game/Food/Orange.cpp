#include "Orange.h"
#include "AnimationPlayer.h"
#include "StateMachine.h"

#include "NotEatenState.h"
#include "HalfEatenState.h"

#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "AnimationComponent.h"
#include "StatsComponent.h"

bool Orange::Init()
{
	transformComponent = AddComponent<TransformComponent>();
	animationComponent = AddComponent<AnimationComponent>();
	statsComponent = AddComponent<StatsComponent>();
	spriteRendererComponent = AddComponent<SpriteRendererComponent>();
	spriteRendererComponent->SetOwner(this);


	transformComponent->Init();
	animationComponent->Init();
	statsComponent->Init();
	spriteRendererComponent->Init();

    if (!InitAnimations())
        return false;
    if (!InitStats())
        return false;
	if (!InitStates())
		return false;

	tastiness = 2.f; // I HATE ORANGES!!!
	healthiness = 7.f;

    return true;
}

void Orange::Start()
{
	transformComponent->Init();
	animationComponent->Init();
	statsComponent->Init();
	spriteRendererComponent->Init();

	InitStats();

	stateMachine->SwitchState(static_cast<int>(EatenState::NotEatenState));
	animationComponent->animPlayer.SwitchAnimation(static_cast<int>(EatenState::NotEatenState));
}

void Orange::Update(const float deltaTime)
{
	stateMachine->Update(deltaTime);
	//animPlayer->Update(currentSprite, deltaTime);
	animationComponent->animPlayer.Update(spriteRendererComponent->sprite, deltaTime);
	spriteRendererComponent->Update(deltaTime);

}

void Orange::Render()
{
	spriteRendererComponent->Render();

}

bool Orange::InitAnimations()
{
	//if (animPlayer)
	//{
	//	animPlayer->SwitchAnimation(static_cast<int>(EatenState::NotEatenState));
	//	return true;
	//}

	//animPlayer = std::make_unique<AnimationPlayer>();
	std::vector<std::unique_ptr<Sprite>> idleAnim;
	idleAnim.reserve(3);

	std::unique_ptr<Sprite> s0 = std::make_unique<Sprite>();
	if (!s0->Init("Sprites/Orange1.dds")) 
		return false;

	std::unique_ptr<Sprite> s1 = std::make_unique<Sprite>();
	if (!s1->Init("Sprites/Orange2.dds")) 
		return false;

	std::unique_ptr<Sprite> s2 = std::make_unique<Sprite>();
	if (!s2->Init("Sprites/Orange3.dds")) 
		return false;

	idleAnim.push_back(std::move(s0));
	idleAnim.push_back(std::move(s1));
	idleAnim.push_back(std::move(s2));

	for (auto& anim : idleAnim)
	{
		anim->SetPosition({ 500,200 });
		anim->SetScale({ 1.f, 1.f });
		anim->SetColor({ 1,1,1,1 });
	}

	animationComponent->animPlayer.InitAnimation(static_cast<int>(EatenState::NotEatenState), std::move(idleAnim));
	animationComponent->animPlayer.SwitchAnimation(static_cast<int>(EatenState::NotEatenState));

	// Second animation
	std::vector<std::unique_ptr<Sprite>> halfAnim;
	halfAnim.reserve(3);

	std::unique_ptr<Sprite> t0 = std::make_unique<Sprite>();
	if (!t0->Init("Sprites/OrangeHalf1.dds"))
		return false;

	std::unique_ptr<Sprite> t1 = std::make_unique<Sprite>();
	if (!t1->Init("Sprites/OrangeHalf2.dds"))
		return false;

	std::unique_ptr<Sprite> t2 = std::make_unique<Sprite>();
	if (!t2->Init("Sprites/OrangeHalf3.dds"))
		return false;

	halfAnim.push_back(std::move(t0));
	halfAnim.push_back(std::move(t1));
	halfAnim.push_back(std::move(t2));

	for (auto& anim : halfAnim)
	{
		anim->SetPosition({ 500,200 });
		anim->SetScale({ 1.f, 1.f });
		anim->SetColor({ 1,1,1,1 });
	}

	spriteRendererComponent->sprite.SetScale({ 1.f, 1.f });
	transformComponent->scale = { 1,1 };
	spriteRendererComponent->sprite.SetColor({ 1,1,1,1 });


	animationComponent->animPlayer.InitAnimation(static_cast<int>(EatenState::HalfEatenState), std::move(halfAnim));
    return true;
}

bool Orange::InitStats()
{
	//stats.HP = stats.MaxHP;
	//statsComponent->stats.HP = statsComponent->stats.MaxHP;
	statsComponent->stats.HP = 100;
	statsComponent->stats.MaxHP = 100;

    return true;
}

bool Orange::InitStates()
{
	if (stateMachine)
	{
		stateMachine->SwitchState(static_cast<int>(EatenState::NotEatenState));
		return true;
	}

	stateMachine = std::make_unique<StateMachine>();
	stateMachine->currentState = stateMachine->Initialize<NotEatenState>(this, "NotEatenState", static_cast<int>(EatenState::NotEatenState), 3);

	stateMachine->Initialize<HalfEatenState>(this, "HalfEatenState", static_cast<int>(EatenState::HalfEatenState), 3);

	stateMachine->SwitchState(static_cast<int>(EatenState::NotEatenState));

	return true;
}
