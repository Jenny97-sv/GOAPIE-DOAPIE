#include "NotEatenState.h"
#include "GameObject.h"
#include "Food.h"

#include "StatsComponent.h"

bool NotEatenState::Start(GameObject* host)
{
	timer = 0;
	maxTimer = 10;
	//statsComponent = host->GetComponent<StatsComponent>();
	if (host->TryGetComponent<StatsComponent>(statsComponent))
		return  true;
	return false;
}

bool NotEatenState::Update(GameObject* host, const float deltaTime)
{
	timer += deltaTime;
	t = timer / maxTimer;
	//if (t > 0.3f)
	//{
	//	host->stats.HP = host->stats.MaxHP / 3;
	//}
	return statsComponent->stats.HP / statsComponent->stats.MaxHP < 0.5f;
}

void NotEatenState::End(GameObject* host)
{
	if (Food* food = dynamic_cast<Food*>(host))
	{
		food->GetStateMachine()->SwitchState(static_cast<int>(EatenState::HalfEatenState));
	}
}

void NotEatenState::Interrupt(GameObject* host)
{
}
