#include "PlayerFeed.h"
#include "GameObject.h"
#include "Player.h"
#include "BoidMath.h"
#include "PostMaster.h"
#include "ObjectPoolManager.h"
#include "SceneManager.h"
#include "Random.h"

#include "Orange.h"
#include "Banana.h"
#include "Apple.h"
#include "Chocolate.h"

#include "CommonUtilites/include/InputHandler.h"
#include "Engine.h"


#include "TransformComponent.h"

bool PlayerFeed::Start(GameObject* host)
{
	player = dynamic_cast<Player*>(host);
	if (!player)
		return false;

	transformComponent = player->GetComponent<TransformComponent>();
	Vector2f position = transformComponent->position;

	timer = 0.0f;
	maxTimer = 10.0f;

	random = Random::GetRandomInt(0, 3);
	//random = 0;
	std::cout << random << std::endl;
	static bool poolRegistered = false;
	if (!poolRegistered)
	{
		ObjectPoolManager::GetInstance().RegisterPool<Apple>("Apple", 1); // They can't be called same thing...
		ObjectPoolManager::GetInstance().RegisterPool<Orange>("Orange", 1);
		ObjectPoolManager::GetInstance().RegisterPool<Banana>("Banana", 1);
		ObjectPoolManager::GetInstance().RegisterPool<Chocolate>("Chocolate", 1);

		poolRegistered = true;
	}

	switch (random)
	{
	case 0:
	{
		if (Orange* orange = ObjectPoolManager::GetInstance().Get<Orange>("Orange"))
		{
			//orange->WorldPosition() = player->WorldPosition() + player->GetDirection() * forwardOffset;
			orange->GetComponent<TransformComponent>()->position = position + player->GetDirection() * forwardOffset;
			orange->isActive = true;

			SceneManager::GetInstance().GetActiveScene()->CreateObject(orange);

			food = orange; // store reference
		}
		break;
	}

	case 1:
	{
		if (Apple* apple = ObjectPoolManager::GetInstance().Get<Apple>("Apple"))
		{
			//apple->WorldPosition() = player->WorldPosition() + player->GetDirection() * forwardOffset;
			apple->GetComponent<TransformComponent>()->position = position + player->GetDirection() * forwardOffset;

			apple->isActive = true;

			SceneManager::GetInstance().GetActiveScene()->CreateObject(apple);
			food = apple;
		}
		break;
	}

	case 2:
	{
		if (Banana* banana = ObjectPoolManager::GetInstance().Get<Banana>("Banana"))
		{
			//banana->WorldPosition() = player->WorldPosition() + player->GetDirection() * forwardOffset;
			banana->GetComponent<TransformComponent>()->position = position + player->GetDirection() * forwardOffset;

			banana->isActive = true;

			SceneManager::GetInstance().GetActiveScene()->CreateObject(banana);
			food = banana;
		}
		break;
	}

	case 3:
	{
		if (Chocolate* choc = ObjectPoolManager::GetInstance().Get<Chocolate>("Chocolate"))
		{
			//choc->WorldPosition() = player->WorldPosition() + player->GetDirection() * forwardOffset;
			choc->GetComponent<TransformComponent>()->position = position + player->GetDirection() * forwardOffset;

			choc->isActive = true;

			SceneManager::GetInstance().GetActiveScene()->CreateObject(choc);
			food = choc;
		}
		break;
	}
	}

	Message msg;
	msg.msg = eMessage::ePlayerFeed;
	msg.data = food;
	PostMaster::GetInstance().SendMsg(msg); 

	return true;
}


bool PlayerFeed::Update(GameObject* host, const float deltaTime)
{
	timer += deltaTime;
	t = timer / maxTimer;

	// Maybe be able to take back if it's bad food? Or it's good and boids want it, making them angrier?
	if (t > 0.1f && GetAsyncKeyState('V')) // Don't have an inputwrapper, so have to hardcode like this...
	{
		return true;
	}

	if (!food || !food->isActive)
	{
		return true;
	}

	return t >= 1.0f;
}

void PlayerFeed::End(GameObject* host)
{
	SceneManager::GetInstance().GetActiveScene()->RemoveObject(food);
	switch (random)
	{
	case 0:
		ObjectPoolManager::GetInstance().Return<Orange>("Orange", static_cast<Orange*>(food));
		break;

	case 1:
		ObjectPoolManager::GetInstance().Return<Apple>("Apple", static_cast<Apple*>(food));
		break;

	case 2:
		ObjectPoolManager::GetInstance().Return<Banana>("Banana", static_cast<Banana*>(food));
		break;

	case 3:
		ObjectPoolManager::GetInstance().Return<Chocolate>("Chocolate", static_cast<Chocolate*>(food));
		break;
	}
}


void PlayerFeed::Interrupt(GameObject* host)
{
	SceneManager::GetInstance().GetActiveScene()->RemoveObject(food);
	switch (random)
	{
	case 0:
		ObjectPoolManager::GetInstance().Return<Orange>("Orange", static_cast<Orange*>(food));
		break;

	case 1:
		ObjectPoolManager::GetInstance().Return<Apple>("Apple", static_cast<Apple*>(food));
		break;

	case 2:
		ObjectPoolManager::GetInstance().Return<Banana>("Banana", static_cast<Banana*>(food));
		break;

	case 3:
		ObjectPoolManager::GetInstance().Return<Chocolate>("Chocolate", static_cast<Chocolate*>(food));
		break;
	}
}
