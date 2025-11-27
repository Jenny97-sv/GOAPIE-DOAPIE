#pragma once
#include <random>

namespace Random
{
	int GetRandomInt(int aMin, int aMax);

	template<typename T> T GetRandomNumber(T aMin, T aMax);

	template<typename T>
	T GetRandomNumber(T aMin, T aMax)
	{
		std::random_device randomDiceDevice;
		std::mt19937 rng(randomDiceDevice());
		std::uniform_real_distribution<T> uniformDistance(aMin, aMax);

		T outcome = uniformDistance(rng);
		return outcome;
	}
}