#include "StateMachine.h"

void StateMachine::DeleteStates()
{
	for (auto& state : states)
	{
		if (state.second)
		{
			delete state.second;
			state.second = nullptr;
		}
	}

	states.clear();
}

void StateMachine::SwitchState(int newState)
{
	if (states[newState]->coolDown > 0)
	{
		//std::cout << "State nr " << newState << " is still in coolDown! " << states[newState]->coolDown << std::endl;
		return;
	}
	if (currentState)
	{
		if (onEnd)
			currentState->End(hostie);
		else
			currentState->Interrupt(hostie);
	}
	currentState = nullptr;
	onEnd = false;

	currentState = states[newState];
	currentState->coolDown = currentState->maxCoolDown;
	if (!currentState->Start(hostie))
	{
		std::cout << "Failed to start " << newState << " state. No state active" << std::endl;
		currentState = nullptr;
	}
}

void StateMachine::Update(const float deltaTime)
{
	for (auto& state : states) // Don't want to use auto, but can't make it work otherwise....
	{
		if (state.second->coolDown > 0)
		{
			state.second->coolDown -= deltaTime;
		}
		else
		{
			state.second->coolDown = 0;
		}
	}

	if (currentState && currentState->Update(hostie, deltaTime))
	{
		currentState->End(hostie);
		currentState = nullptr;
		onEnd = true; // So entities can know when a state is finished
	}
}
