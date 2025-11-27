#pragma once
#include "BehaviorTree.h"

class SequenceNode : public BehaviorTreeNode
{
public:
	void AddChild(BehaviorTreeNodePtr child);
	eNodeStatus TryAndTick(const float deltaTime) override;

private:
	std::vector<BehaviorTreeNodePtr> children;
	int currentNode;
};