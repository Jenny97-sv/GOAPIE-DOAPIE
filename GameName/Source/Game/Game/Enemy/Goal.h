#pragma once
#include <string>
#include <algorithm>
//#include <utility>
//#include <unordered_map>
#include <array>


enum class eWorldKey : size_t 
{
	eHP,
	eHasTakenDamage,
	eIsAlerted,
	eIsLookingForPlayer,
	eFriendValue,
	eEat,
	eCircleAroundPlayer,
	eHasFormationSlot,
	eCount
};

using WorldState = std::array<float, static_cast<size_t>(eWorldKey::eCount)>;

enum class eBoidBehaviorType
{
	eFlock,
	eArmy,
	eSpiral,
	eEat,
	eAttack,
	eNone
};

inline float GetWorldValue(const WorldState& ws, eWorldKey key) 
{
	return ws[(size_t)key];
}

inline void SetWorldValue(WorldState& ws, eWorldKey key, float value) 
{
	ws[(size_t)key] = value;
}

class Goal
{
public:
	Goal()
	{
		desiredState.fill(-9999.f); // means "unused"
	}

	void AddCondition(eWorldKey key, float targetValue)
	{
		desiredState[(size_t)key] = targetValue;
	}

	bool IsAchieved(const WorldState& worldState)
	{
		for (size_t i = 0; i < desiredState.size(); i++) 
		{
			if (desiredState[i] != -9999.f)
			{ // Only check used keys
				if (worldState[i] != desiredState[i])
					return false;
			}
		}
		return true;
	}

	const WorldState& GetConditions() const { return desiredState; }

	std::string name = "";
private:
	WorldState desiredState;
};

class GoalAction
{
public:
	float cost = 1.f;
	float effectiveness = 1.f;
	eBoidBehaviorType behavior = eBoidBehaviorType::eNone;
	std::string name = "";

	GoalAction()
	{
		preconditions.fill(-9999.f);
		effects.fill(-9999.f);
	}

	void AddPrecondition(eWorldKey key, float value)
	{
		preconditions[(size_t)key] = value;
	}

	void AddEffect(eWorldKey key, float value)
	{
		effects[(size_t)key] = value;
	}

	bool ArePreconditionsMet(const WorldState& worldState) const
	{
		for (size_t i = 0; i < preconditions.size(); i++) 
		{
			if (preconditions[i] != -9999.f)
			{ // only used values
				if (worldState[i] != preconditions[i])
					return false;
			}
		}
		return true;
	}

	float GetUtility() const
	{
		float scaledEffect = std::clamp(effectiveness, 0.0f, 1.0f);
		float scaledCost = std::clamp(cost, 0.1f, 5.0f);
		return (scaledEffect * 10.0f) / scaledCost;
	}

	WorldState preconditions;
	WorldState effects;
};
