#include "Player.h"
#include "AnimationPlayer.h"
#include "PostMaster.h"

#include "PlayerDash.h"
#include "PlayerMove.h"
#include "PlayerSmile.h"
#include "PlayerPet.h"
#include "PlayerFeed.h"

#include "BoidMath.h"

#include "AnimationComponent.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

Player::~Player()
{
	PostMaster::GetInstance().UnSubscribe(this);
}

bool Player::Init()
{
	std::cout << "Hej!" << std::endl;
	animationComponent = AddComponent<AnimationComponent>();
	spriteRendererComponent = AddComponent<SpriteRendererComponent>();
	transformComponent = AddComponent<TransformComponent>();

	animationComponent->SetOwner(this);
	spriteRendererComponent->SetOwner(this);
	transformComponent->SetOwner(this);

	animationComponent->Init();
	spriteRendererComponent->Init();
	transformComponent->Init();

	if (!InitAnimations())
		return false;
	if (!InitStates())
		return false;

	PostMaster::GetInstance().Subscribe(this);

	return true;
}
void Player::Start()
{
}
void Player::Update(const float deltaTime)
{
	stateMachine->Update(deltaTime);
	BoidMath::Bounds(transformComponent->position);

	if (stateMachine->OnEnd())
	{
		stateMachine->SwitchState(static_cast<int>(PlayerStates::eWalking));
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		stateMachine->SwitchState(static_cast<int>(PlayerStates::eDash));
	}
	else if (GetAsyncKeyState(VK_SHIFT))
	{
		stateMachine->SwitchState(static_cast<int>(PlayerStates::eSmile));
	}
	else if (GetAsyncKeyState('C'))
	{
		stateMachine->SwitchState(static_cast<int>(PlayerStates::ePet));
	}
	else if (GetAsyncKeyState('V'))
	{
		stateMachine->SwitchState(static_cast<int>(PlayerStates::eFeed));
	}

	spriteRendererComponent->Update(deltaTime);
	animationComponent->animPlayer.Update(spriteRendererComponent->sprite, deltaTime);
	//animPlayer->Update(currentSprite, deltaTime);
}
void Player::Render()
{
	spriteRendererComponent->Render();
}

bool Player::InitAnimations()
{
	//if (animPlayer)
	//{
	//	std::cout << "AnimPlayer already exists" << std::endl;
	//	return true;
	//}

	//animPlayer = std::make_unique<AnimationPlayer>();

	std::vector<std::unique_ptr<Sprite>> idleAnim;
	idleAnim.reserve(4);

	std::unique_ptr<Sprite> s0 = std::make_unique<Sprite>();
	if (!s0->Init("Sprites/Player_Normal.dds")) return false;

	std::unique_ptr<Sprite> s1 = std::make_unique<Sprite>();
	if (!s1->Init("Sprites/Player_Dash.dds")) return false;

	std::unique_ptr<Sprite> s2 = std::make_unique<Sprite>();
	if (!s2->Init("Sprites/Player_Sword.dds")) return false;

	std::unique_ptr<Sprite> s3 = std::make_unique<Sprite>();
	if (!s3->Init("Sprites/Player_Throw.dds")) return false;

	idleAnim.push_back(std::move(s0));
	idleAnim.push_back(std::move(s1));
	idleAnim.push_back(std::move(s2));
	idleAnim.push_back(std::move(s3));

	for (auto& anim : idleAnim)
	{
		anim->SetScale({ 0.2f,0.2f });
		anim->SetPosition({ 0,0 });
		anim->SetColor({ 1,1,1,1 });
	}
	spriteRendererComponent->sprite.SetScale({ 0.2f,0.2f });
	spriteRendererComponent->sprite.SetColor({ 1,1,1,1 });
	transformComponent->scale = { 0.2f,0.2f };
	transformComponent->position = { 10,10 };
	transformComponent->rotation = 0;
	//currentSprite.SetScale({ 0.2f,0.2f });

	animationComponent->animPlayer.InitAnimation(static_cast<int>(AnimNames::eIdle), std::move(idleAnim));
	animationComponent->animPlayer.SwitchAnimation(static_cast<int>(AnimNames::eIdle));
	//animPlayer->InitAnimation(static_cast<int>(AnimNames::eIdle), idleAnim);
	//animPlayer->SwitchAnimation(static_cast<int>(AnimNames::eIdle));

	return true;
}


bool Player::InitStates()
{
	if (stateMachine)
	{
		std::cout << "State machine already exists!" << std::endl;
		return true;
	}
	stateMachine = std::make_unique<StateMachine>();
	// Must set current state to something in beginning... not good...
	stateMachine->Initialize<PlayerDash>(this, "PlayerDash", static_cast<int>(PlayerStates::eDash), 2);
	stateMachine->currentState = stateMachine->Initialize<PlayerMove>(this, "PlayerMove", static_cast<int>(PlayerStates::eWalking), 0);

	stateMachine->Initialize<PlayerSmile>(this, "PlayerSmile", static_cast<int>(PlayerStates::eSmile), 2);

	stateMachine->Initialize<PlayerPet>(this, "PlayerPet", static_cast<int>(PlayerStates::ePet), 2);

	stateMachine->Initialize<PlayerFeed>(this, "PlayerFeed", static_cast<int>(PlayerStates::eFeed), 2);

	stateMachine->SwitchState(static_cast<int>(PlayerStates::eWalking));

	return true;
}

void Player::OnMessageSent(const Message msg)
{
}
