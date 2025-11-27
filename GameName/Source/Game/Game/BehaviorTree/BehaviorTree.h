#pragma once
#include <vector>
//#include <unordered_set>
#include <iostream>
class Action;

enum class eNodeStatus
{
	eSuccess,
	eFailure,
	eRunning,
	eInvalid
};

using BehaviorTreeNodePtr = std::unique_ptr<class BehaviorTreeNode>;
using ActionPtr = std::unique_ptr<class Action>;


class BehaviorTreeNode
{
public:
    virtual ~BehaviorTreeNode() = default;
    virtual eNodeStatus TryAndTick(const float deltaTime) = 0;
};

class BehaviorTree
{
public:
    void Tick(const float deltaTime)
    {
        if (root) root->TryAndTick(deltaTime);
    }

    eNodeStatus TickAction(Action* action, float deltaTime, bool shouldInterrupt);

    void SetRoot(BehaviorTreeNodePtr node)
    {
        root = std::move(node);
    }


    void Reset()
    {
        isActionRunning = false;
    }

private:
    BehaviorTreeNodePtr root = nullptr;
    bool isActionRunning = false;
};
