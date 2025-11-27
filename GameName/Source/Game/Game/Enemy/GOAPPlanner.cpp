#include "GOAPPlanner.h"
#include <ostream>
#include <algorithm>
//#include "BoidMath.h"
#include <sstream>
#include <iostream>

Goal* GOAPPlanner::ChooseGoal(std::vector<Goal>& goals, const WorldState& worldState)
{
	for (Goal& g : goals)
	{
		if (!g.IsAchieved(worldState))
			return const_cast<Goal*>(&g);
	}
	return nullptr;
}

std::deque<GoalAction*> GOAPPlanner::MakeGoodPlan(WorldState world, std::vector<GoalAction>& actions, Goal& goal)
{
    std::deque<GoalAction*> candidates;

    for (GoalAction& a : actions)
    {
        if (a.ArePreconditionsMet(world))
            candidates.push_back(&a);
    }

    if (candidates.empty())
        return {};


    std::sort(candidates.begin(), candidates.end(),
        [](GoalAction* a, GoalAction* b)
        {
            return a->GetUtility() > b->GetUtility();
        });

    // Debug stuff!
    //int i = 0;
    //for (GoalAction*& can : candidates)
    //{
    //    //std::cout << candidates.at(i)->name << " " << can->GetUtility() << std::endl;
    //    i++;
    //}

    return candidates;
}


std::deque<GoalAction*> GOAPPlanner::MakePlan(WorldState world, std::vector<GoalAction>& goalActions, Goal& goal)
{
    std::deque<GoalAction*> plan; 
    for (GoalAction& action : goalActions)
    {
        if (action.ArePreconditionsMet(world))
        {
            ApplyEffects(world, action.effects);
            plan.push_back(const_cast<GoalAction*>(&action));
            if (goal.IsAchieved(world))
                break;
        }
    }
    return plan;
}

void GOAPPlanner::ApplyEffects(WorldState& world, const WorldState& eff)
{
    for (size_t i = 0; i < (size_t)eWorldKey::Count; i++)
    {
        if (eff[i] != -1.0f)   // -1 means “no effect”
            world[i] = eff[i];
    }
}


std::string GOAPPlanner::SerializeWorldState(const WorldState& world) // This wont work!
{
    return std::string();
    std::vector<std::string> keys;
    keys.reserve(world.size());
    //for (const auto& [key, _] : world)
    //    keys.push_back(key);
    //for (auto& i : world)
    //{
    //    keys.push_back(i);
    //}

    std::sort(keys.begin(), keys.end());

    std::ostringstream oss;
    for (const auto& key : keys)
        //oss << key << ":" << world.at(key) << ";";

    return oss.str();
}
