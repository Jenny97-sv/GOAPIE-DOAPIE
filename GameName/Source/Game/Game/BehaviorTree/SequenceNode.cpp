#include "SequenceNode.h"

void SequenceNode::AddChild(BehaviorTreeNodePtr child)
{
	children.push_back(std::move(child));
}

eNodeStatus SequenceNode::TryAndTick(const float deltaTime)
{
    if (currentNode >= children.size()) 
    {
        currentNode = 0; // Reset for next time
        return eNodeStatus::eSuccess;
    }

    eNodeStatus status = children[currentNode]->TryAndTick(deltaTime);

    if (status == eNodeStatus::eFailure)
    {
        currentNode = 0; 
        return status;
    }
    else if (status == eNodeStatus::eSuccess)
    {
        currentNode++;
        if (currentNode >= children.size())
        {
            currentNode = 0; 
            return eNodeStatus::eSuccess;
        }
        return eNodeStatus::eRunning; 
    }

    return status; 
}
