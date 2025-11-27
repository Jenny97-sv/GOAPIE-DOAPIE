#pragma once
#include <numbers>
#include "CommonUtilites/Vector/Vector2.h"
//#include <cmath>

namespace BoidMath
{
	constexpr double PI = std::numbers::pi;
	constexpr float Pi = static_cast<float>(PI);
	inline Vector2ui resolution = { 1280, 720}; // Shouldn't be like this, public for anyone but whatever
	inline Vector2f resolutionFloat = static_cast<Vector2f>(resolution);

	inline void Bounds(Vector2f& position)
	{
		float& x = position.x;
		float& y = position.y;

		float xMax = resolutionFloat.x;
		float yMax = resolutionFloat.y;

		if (x < 0)
		{
			x = xMax - 1;
		}
		else if (x > xMax + 1)
		{
			x = 1;
		}

		if (y < 0)
		{
			y = yMax - 1;
		}
		else if (y > yMax + 1)
		{
			y = 1;
		}
	}

	inline float Lerp(float aStart, float anEnd, float t)
	{
		return aStart + (anEnd - aStart) * t;
	}
	inline Vector2f Lerp(Vector2f aStart, Vector2f anEnd, float t)
	{
		return aStart + (anEnd - aStart) * t;
	}

	template<typename T>
	inline T Lerp(T aStart, T anEnd, T t)
	{
		return aStart + (anEnd - aStart) * static_cast<T>(t);
	}

	inline Vector2f Lemniscate(float t, float a = 1)
	{
		float denom = 1 + std::sin(t) * std::sin(t);
		float x = (a * std::cos(t)) / denom;
		float y = (a * std::sin(t) * std::cos(t)) / denom;
		return { x, y };
	}

	inline float SineWave(float time, float frequency = 1.0f, float phase = 0.0f)
	{
		return std::sin(time * frequency * Pi * 2.0f + phase);
	}

	inline float SineOscillate(float time, float minVal, float maxVal, float frequency = 1.0f, float phase = 0.0f)
	{
		float s = (std::sin(time * frequency * Pi * 2.0f + phase) + 1.0f) * 0.5f; // [0,1]
		return Lerp(minVal, maxVal, s);
	}

	template<typename T>
	inline T SineWave(T time, T frequency = 1, T phase = 0)
	{
		return std::sin(time * frequency * Pi * 2 + phase);
	}

	template<typename T>
	inline T SineOscillate(T time, T minVal, T maxVal, T frequency = 1, T phase = 0)
	{
		T s = (std::sin(time * frequency * Pi * 2 + phase) + 1) / 2; 
		return Lerp(minVal, maxVal, s);
	}

	template<typename T>
	inline constexpr T DegToRad(T degrees)
	{
		return degrees * static_cast<T>(PI / 180.0);
	}

	template<typename T>
	inline constexpr T RadToDeg(T radians)
	{
		return radians * static_cast<T>(180.0 / PI);
	}

	template<typename T>
	inline bool Ascending(T first, T second)
	{
		return first < second;
	}

	template<typename T>
	inline bool Descending(T first, T second)
	{
		return first > second;
	}
}