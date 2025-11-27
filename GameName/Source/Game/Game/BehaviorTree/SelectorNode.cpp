#include "SelectorNode.h"

eNodeStatus SelectorNode::TryAndTick(const float deltaTime)
{
	for (BehaviorTreeNodePtr& child : children)
	{
		eNodeStatus status = child->TryAndTick(deltaTime);
		if (status == eNodeStatus::eSuccess)
			return status;
		if (status == eNodeStatus::eRunning)
			return status;
		//return child->TryAndTick(deltaTime);
	}
    return eNodeStatus::eInvalid;
}

void SelectorNode::AddChild(BehaviorTreeNodePtr child)
{
	if (std::find(children.begin(), children.end(), child) != children.end())
	{
		std::cout << "Child already existed!" << std::endl;
		return;
	}

    children.push_back(std::move(child));
}
