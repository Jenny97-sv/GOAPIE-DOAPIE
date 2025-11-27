#pragma once
#include "CommonUtilites\Vector\Vector.h"
#include <unordered_map>

class GameObject;
class Player;

class State
{
public:
	virtual bool Start(GameObject* host) = 0;
	virtual bool Update(GameObject* host, const float deltaTime) = 0;
	virtual void End(GameObject* host) = 0;
	virtual void Interrupt(GameObject* host) = 0;
	
protected:
	Player* player = nullptr;
	float timer = 0;
	float maxTimer = 1;
	float t = 0;
private:
	float coolDown = 0;
	float maxCoolDown = 1;
	int id = -1;
	std::string name = "";
	friend class StateMachine;
};

class StateMachine
{
public:
	State* currentState = nullptr;
	template <typename T, typename... Args>
	State* Initialize(GameObject* host, std::string name, int id, float coolDown, Args&&... args)
	{
		static_assert(std::is_base_of<State, T>::value, "T must derive from State");

		if (states.contains(id))
		{
			std::cout << "State " << id << " already exists" << std::endl;
			return nullptr;
		}

		T* state = new T(std::forward<Args>(args)...);

		state->id = id;
		state->name = name;
		state->maxCoolDown = coolDown;
		state->coolDown = 0;

		hostie = host;
		states.insert({ id, state });
		return state;
	}

	void DeleteStates();

	void SwitchState(int newState);
	void Update(const float deltaTime);

	bool OnEnd() { return onEnd; }
private:
	std::unordered_map<int, State*> states; // int used, every gameobject can use their own enum
	GameObject* hostie = nullptr;
	bool onEnd = false;
};