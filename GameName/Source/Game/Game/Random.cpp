#include "Random.h"

int Random::GetRandomInt(int aMin, int aMax)
{
	std::random_device randomDiceDevice;
	std::mt19937 rng(randomDiceDevice());
	std::uniform_int_distribution<int> uniformDistance(aMin, aMax);

	int outcome = uniformDistance(rng);
	return outcome;
}
