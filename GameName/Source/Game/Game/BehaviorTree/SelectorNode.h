#pragma once
#include "BehaviorTree.h"
class SelectorNode : public BehaviorTreeNode
{
public:
	eNodeStatus TryAndTick(const float deltaTime) override;
	void AddChild(BehaviorTreeNodePtr child);

private:
	std::vector<BehaviorTreeNodePtr> children;
};