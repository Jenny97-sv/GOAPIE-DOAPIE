#include "BoidyIdle.h"

bool BoidyIdle::Start()
{
	timer = 0;
	maxTimer = 3;
	return true;
}

eNodeStatus BoidyIdle::Update(const float deltaTime)
{
	timer += deltaTime;
	t = timer / maxTimer;
	if (t >= 1)
		return eNodeStatus::eSuccess;
	else
		return eNodeStatus::eRunning;
}

void BoidyIdle::End()
{
	timer = 0;
}

void BoidyIdle::Interrupt()
{
	timer = 0;
}
