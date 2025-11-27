//#include "Goal.h"
//#include <iostream>
//
//void Goal::AddCondition(const std::string& key, const float targetValue)
//{
//	if (desiredState.find(key) != desiredState.end())
//	{
//		std::cout << key << " already exists!" << std::endl;
//		return;
//	}
//
//	desiredState[key] = targetValue;
//}
//
//bool Goal::IsAchieved(const WorldState worldState)
//{
//	if (desiredState.empty())
//		return false;
//
//	for (auto& [key, value] : desiredState)
//	{
//		auto it = worldState.find(key);
//		if (it == worldState.end() || it->second != value)
//			return false;
//	}
//
//	return true;
//}
