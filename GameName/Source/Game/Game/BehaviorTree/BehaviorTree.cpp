#include "BehaviorTree.h"
#include "ActionNode.h"

eNodeStatus BehaviorTree::TickAction(Action* action, float deltaTime, bool shouldInterrupt)
{
	if (!action)
		return eNodeStatus::eInvalid;

	if (!isActionRunning)
	{
		if (action->Start())
			isActionRunning = true;
		else
			action->Interrupt();
	}
	else
	{
		if (shouldInterrupt)
		{
			action->Interrupt();
			isActionRunning = false;
			return eNodeStatus::eFailure;
		}

		eNodeStatus status = action->Update(deltaTime);

		switch (status)
		{
		case eNodeStatus::eSuccess:
			action->End();
			isActionRunning = false;
			break;

		case eNodeStatus::eFailure:
			action->Interrupt();
			isActionRunning = false;
			break;
		}
		return status;

	}
	return eNodeStatus::eRunning;
}
