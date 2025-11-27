#include "PlayerDash.h"
#include "GameObject.h"
#include "Player.h"
#include "PostMaster.h"

#include "TransformComponent.h"

bool PlayerDash::Start(GameObject* host)
{
	timer = 0;
	maxTimer = 0.3f;
	distance = 200;
	if (player = dynamic_cast<Player*>(host))
	{
		Message msg;
		msg.msg = eMessage::ePlayerDashed;
		PostMaster::GetInstance().SendMsg(msg);

		transformComponent = host->GetComponent<TransformComponent>();
		//startPosition = host->WorldPosition();
		startPosition = transformComponent->position;
		endPosition = startPosition + player->GetDirection() * distance;
		return true;
	}


	return false;
}

bool PlayerDash::Update(GameObject* host, const float deltaTime)
{
	timer += deltaTime;
	t = timer / maxTimer;

	//host->WorldPosition() = Vector2f::Lerp(startPosition, endPosition, t);
	transformComponent->position = Vector2f::Lerp(startPosition, endPosition, t);

	return t >= 1;
}

void PlayerDash::End(GameObject* host)
{
	//host->WorldPosition() = endPosition;
	transformComponent->position = endPosition;
	timer = 0;
}

void PlayerDash::Interrupt(GameObject* host)
{
	timer = 0;
}
