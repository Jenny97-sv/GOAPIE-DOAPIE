#pragma once
#include "StateMachine.h"
//template <typename T, typename... Args>
//State* StateMachine::Initialize(GameObject* host, std::string name, int id, float coolDown, Args&&... args)
//{
//	// friend access works here reliably
//	T* state = new T(std::forward<Args>(args)...);
//	state->id = id;
//	state->name = name;
//	state->maxCoolDown = coolDown;
//	state->coolDown = 0;
//
//	hostie = host;
//	states.insert({ id, state });
//	return state;
//}
