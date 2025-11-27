#include "BoidyWalk.h"
#include "GameObject.h"
#include "Random.h"

bool BoidyWalk::Start()
{
	direction.x = Random::GetRandomNumber(-1.f, 1.f);
	direction.y = Random::GetRandomNumber(-1.f, 1.f);
	distance = 100;

	startPosition = blackboard->GetValue<Vector2f>("Position");
	endPosition = startPosition + direction * distance;

	timer = 0;
	maxTimer = 1;

	return true;
}

eNodeStatus BoidyWalk::Update(const float deltaTime)
{
	timer += deltaTime;
	t = timer / maxTimer;
	blackboard->SetValue("Position", Vector2f::Lerp(startPosition, endPosition, t));

	if (t >= 1)
		return eNodeStatus::eSuccess;
	else
		return eNodeStatus::eRunning;
}

void BoidyWalk::End()
{
	timer = 0;
}

void BoidyWalk::Interrupt()
{
	timer = 0;
}
