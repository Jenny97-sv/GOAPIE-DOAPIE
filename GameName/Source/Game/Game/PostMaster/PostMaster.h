#pragma once
#include <vector>
#include <memory>
#include "Observer.h"

class PostMaster
{
public:
	static PostMaster& GetInstance()
	{
		return *Instance;
	}

	void Subscribe(Observer* ob);
	void UnSubscribe(Observer* ob);

	void SendMsg(Message msg);

	static void CreateInstance()
	{
		Instance = new PostMaster();
	}

	static void DestroyInstance()
	{
		delete Instance;
		Instance = nullptr;
	}
private:
	std::vector<Observer*> observers{};
	static PostMaster* Instance;
};