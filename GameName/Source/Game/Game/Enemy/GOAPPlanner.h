#pragma once
#include <deque>
#include "Goal.h"
#include <vector>
#include <unordered_set>

class GOAPPlanner
{
public:
	static Goal* ChooseGoal(std::vector<Goal>& goals, const WorldState& worldState);

	static std::deque<GoalAction*> MakeGoodPlan(WorldState world, std::vector<GoalAction>& actions, Goal& goal);
    static std::deque<GoalAction*> MakePlan(WorldState world, std::vector<GoalAction>& goalActions, Goal& goal);
private:
	//static bool ArePreconditionsMet(const WorldState& worldState, const WorldState& pre);
	static void ApplyEffects(WorldState& world, const WorldState& eff);
    static std::string SerializeWorldState(const WorldState& world);

    struct Node
    {
        WorldState state;
        GoalAction* action;
        float cost;
        std::vector<GoalAction*> path;
    };
};