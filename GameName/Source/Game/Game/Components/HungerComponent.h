#pragma once
#include "Component.h"
#include <algorithm>

class HungerComponent : public Component
{
public:
    void Update(float dt) override
    {
        hunger += dt * hungerDecreaser;
        hunger = std::clamp(hunger, 0.f, maxHunger);
    }

    const float HungerPercent() const
    {
        return hunger / maxHunger;
    }

    void SetHunger(const float hungerLevel)
    {
        hunger = hungerLevel;
        hunger = std::clamp(hunger, 0.f, maxHunger);
    }

    void AdjustHunger(const float hungerLevel)
    {
        hunger -= hungerLevel;
        hunger = std::clamp(hunger, 0.f, maxHunger);
    }

    const float GetHunger() const
    {
        return hunger;
    }

private:
    float hungerDecreaser = 0.2f;
    float hunger = 0.f;
    const float maxHunger = 10.f;
};
