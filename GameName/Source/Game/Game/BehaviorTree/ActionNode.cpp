#include "ActionNode.h"

eNodeStatus ActionNode::TryAndTick(const float deltaTime)
{
    if (!action)
    {
        return eNodeStatus::eInvalid;
    }

    if (!hasStarted)
    {
        action->Start();
        hasStarted = true;
    }

    eNodeStatus status = action->Update(deltaTime);

    switch (status)
    {
    case eNodeStatus::eSuccess:
        action->End();
        hasStarted = false;
        return status;

    case eNodeStatus::eFailure:
        action->Interrupt();
        hasStarted = false;
        return status;
    }

    return eNodeStatus::eRunning;
}