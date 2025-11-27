#include "ParallellNode.h"

eNodeStatus ParallellNode::TryAndTick(const float deltaTime)
{
    bool allSuccess = true;

    for (BehaviorTreeNodePtr& node : children)
    {
        eNodeStatus status = node->TryAndTick(deltaTime);

        if (status == eNodeStatus::eFailure)
            return eNodeStatus::eFailure;   // one failure kills the parallel

        if (status != eNodeStatus::eSuccess)
            allSuccess = false; // at least one still running
    }

    return allSuccess ? eNodeStatus::eSuccess : eNodeStatus::eRunning;
}


void ParallellNode::AddChild(BehaviorTreeNodePtr child)
{
    if (std::find(children.begin(), children.end(), child) != children.end())
    {
        std::cout << "Child already exists in ParallellNode" << std::endl;
        return;
    }
	children.push_back(std::move(child)); 
}
