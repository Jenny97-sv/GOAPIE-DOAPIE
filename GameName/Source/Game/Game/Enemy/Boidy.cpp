#include "Boidy.h"
#include "PostMaster.h"
#include "Food.h"

#include "BoidyIdle.h"
#include "BoidyWalk.h"

#include "Random.h"
#include "BoidMath.h"


#include "TransformComponent.h"
#include "StatsComponent.h"
#include "AnimationComponent.h"
#include "SpriteRendererComponent.h"

#include "HungerComponent.h"
#include "FriendshipComponent.h"
#include "BoidyAIComponent.h"
#include "MovementComponent.h"

Boidy::~Boidy()
{
	PostMaster::GetInstance().UnSubscribe(this);
}

bool Boidy::Init()
{
	transformComponent = AddComponent<TransformComponent>();
	animationComponent = AddComponent<AnimationComponent>();
	spriteRendererComponent = AddComponent<SpriteRendererComponent>();
	statsComponent = AddComponent<StatsComponent>();

	hungerComponent = AddComponent<HungerComponent>();
	friendshipComponent = AddComponent<FriendshipComponent>();
	aiComponent = AddComponent<BoidyAIComponent>();
	aiComponent->SetOwner(this);
	movementComponent = AddComponent<MovementComponent>();
	movementComponent->SetOwner(this);
	spriteRendererComponent->SetOwner(this);

	transformComponent->Init();
	animationComponent->Init();
	spriteRendererComponent->Init();
	statsComponent->Init();
	aiComponent->Init();
	movementComponent->Init();

	//float randomAcc = Random::GetRandomNumber<float>(1.f, 5.f);
	//movementComponent->SetMaxAcceleration(randomAcc);

	if (!InitAnimations())
		return false;
	if (!InitStats())
		return false;
	if (!InitTransform())
		return false;

	PostMaster::GetInstance().Subscribe(this);

	InitEnemy(100, 50, 400, 200);

	isAlerted = false;

	formationSlot = GetID();

	return true;
}

void Boidy::Start()
{
	statsComponent->stats.HP = statsComponent->stats.MaxHP;
	InitTransform();
	InitStats();
	animationComponent->animPlayer.SwitchAnimation(0);
}

void Boidy::Update(const float deltaTime)
{
	BoidMath::Bounds(transformComponent->position);
	aiComponent->Update(deltaTime);
	hungerComponent->Update(deltaTime);
	spriteRendererComponent->Update(deltaTime);
	animationComponent->animPlayer.Update(spriteRendererComponent->sprite, deltaTime);
	movementComponent->Update(deltaTime);
}

void Boidy::Render()
{
	spriteRendererComponent->Render();
}

bool Boidy::InitAnimations()
{
	std::vector<std::unique_ptr<Sprite>> idleAnim;
	idleAnim.reserve(2);

	std::unique_ptr<Sprite> s0 = std::make_unique<Sprite>();
	if (!s0->Init("Sprites/Bat_Attack1.dds")) return false;

	std::unique_ptr<Sprite> s1 = std::make_unique<Sprite>();
	if (!s1->Init("Sprites/Bat_Attack2.dds")) return false;

	idleAnim.push_back(std::move(s0));
	idleAnim.push_back(std::move(s1));

	for (auto& anim : idleAnim)
	{
		anim->SetPosition({ 500,200 });
		anim->SetScale({ 0.1f, 0.1f });
		anim->SetColor({ 1,1,1,1 });
	}
	spriteRendererComponent->sprite.SetScale({ 0.1f, 0.1f });
	spriteRendererComponent->sprite.SetColor({ 1,1,1,1 });
	transformComponent->scale = { 0.1f, 0.1f };

	animationComponent->animPlayer.InitAnimation(0, std::move(idleAnim));
	animationComponent->animPlayer.SwitchAnimation(0);

	return true;
}

bool Boidy::InitStats()
{
	statsComponent->stats.HP = 100;
	statsComponent->stats.MaxHP = 100;

	statsComponent->stats.Attack = 20;
	statsComponent->stats.MaxAttack = 20;

	statsComponent->stats.Speed = 500;

	return true;
}

bool Boidy::InitTransform()
{
	int randomX = Random::GetRandomInt(100, 1000);
	int randomY = Random::GetRandomInt(100, 500);
	transformComponent->position = { randomX, randomY };

	float randomRot = Random::GetRandomNumber(-1.f, 1.f);
	transformComponent->rotation = randomRot;

	return true;
}

void Boidy::OnMessageSent(const Message msg)
{
	aiComponent->HandleMessage(msg);
}
