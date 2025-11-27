#pragma once
#include "BehaviorTree.h"
#include "Blackboard.h"
#include "CommonUtilites/Vector/Vector.h"


class Player;

class Action
{
public:
    Action(Blackboard* blackboard) : blackboard(blackboard) {}
    virtual ~Action() = default;
	virtual bool Start() = 0;
	virtual eNodeStatus Update(const float deltaTime) = 0;
	virtual void End() = 0;
	virtual void Interrupt() = 0;

protected:
    float timer = 0;
    float maxTimer = 1;
    float t = 0;
    Player* player = nullptr;
    Blackboard* blackboard = nullptr;
};

class ActionNode : public BehaviorTreeNode
{
public:
    ActionNode(ActionPtr act) : action(std::move(act)), hasStarted(false) {}

    eNodeStatus TryAndTick(const float deltaTime) override;

    void Reset() { hasStarted = false; }  // Useful for restarting the action

private:
    ActionPtr action;
    bool hasStarted;
};