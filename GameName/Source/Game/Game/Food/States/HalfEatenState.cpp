#include "HalfEatenState.h"
#include "GameObject.h"
#include "Food.h"
#include "AnimationPlayer.h"
#include "SceneManager.h"

#include "AnimationComponent.h"
#include "StatsComponent.h"

bool HalfEatenState::Start(GameObject* host)
{
	Food* food = dynamic_cast<Food*>(host);
	if (!food)
		return false;

	food->GetAnimationComponent()->animPlayer.SwitchAnimation(static_cast<int>(EatenState::HalfEatenState)); // this will error since animations aren't in yet!!!!!
	statsComponent = food->GetComponent<StatsComponent>();
	return true;
}

bool HalfEatenState::Update(GameObject* host, const float deltaTime)
{
	return statsComponent->stats.HP <= 0;
}

void HalfEatenState::End(GameObject* host)
{
	SceneManager::GetInstance().GetActiveScene()->RemoveObject(host);
}

void HalfEatenState::Interrupt(GameObject* host)
{
}
