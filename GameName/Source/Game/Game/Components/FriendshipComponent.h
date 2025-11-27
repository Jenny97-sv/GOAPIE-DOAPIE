#pragma once
#include "Component.h"
#include <algorithm>

class FriendshipComponent : public Component
{
public:

    void Add(const float amount)
    {
        value += amount;
        Clamp();
    }

    void Subtract(const float amount)
    {
        value -= amount;
        Clamp();
    }

    const float GetCurrent() const
    {
        return value;
    }

    const float GetPercent() const
    {
        return value / maxValue;
    }

    bool HalfOrLess()
    {
        return value <= maxValue / 2;
    }

private:
    float value = 5.f;
    const float maxValue = 10.f;

    void Clamp()
    {
        value = std::clamp(value, 0.f, maxValue);
    }
};
