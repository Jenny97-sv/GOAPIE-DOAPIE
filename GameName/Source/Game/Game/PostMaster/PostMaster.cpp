#include "PostMaster.h"
#include <iostream>


PostMaster* PostMaster::Instance = nullptr; // REQUIRED DEFINITION

void PostMaster::Subscribe(Observer* ob)
{
	if (std::find(observers.begin(), observers.end(), ob) != observers.end())
	{
		std::cout << "Observer already here!" << std::endl;
		return;
	}
	observers.push_back(ob);
}

void PostMaster::UnSubscribe(Observer* ob)
{
	for (size_t i = 0; i < observers.size(); i++)
	{
		if (observers[i] == ob)
		{
			observers.erase(observers.begin() + i);
			return;
		}
	} 

	std::cout << "Couldn't find the observer" << std::endl;
}

void PostMaster::SendMsg(Message msg)
{
	for (Observer*& ob : observers)
	{
		ob->OnMessageSent(msg);
	}
}
