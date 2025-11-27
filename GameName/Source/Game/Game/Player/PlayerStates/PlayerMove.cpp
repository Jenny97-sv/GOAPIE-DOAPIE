#include "PlayerMove.h"
#include "GameObject.h"
#include "Player.h"

#include "TransformComponent.h"

bool PlayerMove::Start(GameObject* host)
{
	if (player = dynamic_cast<Player*>(host))
	{
		//transformComponent = player->GetComponent<TransformComponent>(); return true;
		if (player->TryGetComponent<TransformComponent>(transformComponent))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
}

bool PlayerMove::Update(GameObject* host, const float deltaTime)
{
	player = dynamic_cast<Player*>(host); // Why I have to point these each frame?
	if (!player) return true;

	//previousPosition = host->WorldPosition();
	previousPosition = transformComponent->position;
	if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP))
	{
		//host->WorldPosition() = { previousPosition.x, previousPosition.y - speed * deltaTime };
		transformComponent->position = { previousPosition.x, previousPosition.y - speed * deltaTime };
		player->SetDirection({ 0,-1 });
	}
	if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN))
	{
		//host->WorldPosition() = { previousPosition.x, previousPosition.y + speed * deltaTime };
		transformComponent->position = { previousPosition.x, previousPosition.y + speed * deltaTime };
		player->SetDirection({ 0,1 });
	}
	if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
	{
		//host->WorldPosition() = { previousPosition.x - speed * deltaTime, previousPosition.y };
		transformComponent->position = { previousPosition.x - speed * deltaTime, previousPosition.y };
		player->SetDirection({ -1, 0 });
	}
	if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
	{
		//host->WorldPosition() = { previousPosition.x + speed * deltaTime, previousPosition.y };
		transformComponent->position = { previousPosition.x + speed * deltaTime, previousPosition.y };
		player->SetDirection({ 1,0 });
	}
	return false; // Only when player switches state will this return true
}

void PlayerMove::End(GameObject* host)
{
}

void PlayerMove::Interrupt(GameObject* host)
{
}
