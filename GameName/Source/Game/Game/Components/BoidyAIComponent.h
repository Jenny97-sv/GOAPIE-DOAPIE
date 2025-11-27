#pragma once
#include "Component.h"
#include <memory>
#include <vector>
#include "GOAPPlanner.h"
#include "BehaviorTree.h"
#include "Blackboard.h"
#include "Observer.h"

#include "BoidyFlock.h"
#include "BoidySpiral.h"
#include "BoidyArmy.h"
#include "BoidyEat.h"
#include "BoidyAttack.h"



class TransformComponent;
class StatsComponent;
class FriendshipComponent;
class HungerComponent;
class SpriteRendererComponent;
class Boidy;

class BoidyAIComponent : public Component
{
public:
	void Init() override;
	void Update(const float deltaTime) override;
    void HandleMessage(const Message msg);
private:
    std::unique_ptr<BehaviorTree> behaviorTree = nullptr;
    std::unique_ptr<Blackboard> blackboard = nullptr;
    std::unique_ptr<GOAPPlanner> planner = nullptr;

    std::unique_ptr<BoidyArmy> armyAction = nullptr;
    std::unique_ptr<BoidyFlock> flockAction = nullptr;
    std::unique_ptr<BoidySpiral> spiralAction = nullptr;
    std::unique_ptr<BoidyEat> eatAction = nullptr;
    std::unique_ptr<BoidyAttack> attackAction = nullptr;

    std::vector<Goal> goals;
    std::vector<GoalAction> actions;
    WorldState worldState;
    std::deque<GoalAction*> currentPlan;

    bool BuildGOAP();
    bool BuildBehaviorTree();
    void UpdateWorldState(float dt);
    bool ExecuteBehavior(GoalAction* action, BoidBehaviorType type, float dt);
    void UpdateFriendColor(const float friendshipMeter);

    TransformComponent* transformComponent = nullptr;
    StatsComponent* statsComponent = nullptr;
    FriendshipComponent* friendshipComponent = nullptr;
    HungerComponent* hungerComponent = nullptr;
    SpriteRendererComponent* spriteRendererComponent = nullptr;
    Boidy* boidy = nullptr;
    Food* food = nullptr;

    Vector4f hostileColor = { 1,0,0,1 };
    Vector4f normalColor = { 1,1,1,1 };
    Vector4f friendColor = { 0,1,0,1 };

    bool shouldEat = false;

};