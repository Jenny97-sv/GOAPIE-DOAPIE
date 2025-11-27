#pragma once
#include <bitset>
#include <windows.h>

namespace CommonUtilities
{
	class InputHandler
	{
	public:
		InputHandler();
		~InputHandler();
		bool IsKeyDown(const int aKeyCode) const;
		bool IsKeyPressed(const int aKeyCode) const;
		bool IsKeyReleased(const int aKeyCode)const;

		bool IsMouseButtonDown(const int aKeyCode)const;
		bool IsMouseButtonReleased(const int aKeyCode)const;
		float GetMouseWheelData();
		POINT GetMousePosition() const;
		POINT GetMouseLastPos() const;
		POINT GetMouseDelta() const;

		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

		void Update();


	private:
		float myTentativeMouseWheelDelta;
		float myMouseWheelDelta;

		POINT myMousePos;
		POINT myMouseDelta;
		POINT myMouseLastPos;
		std::bitset<256> myKeysCurrentState{};
		std::bitset<256> myKeysPreviousState{};
		std::bitset<256> myKeysTentativeState{};
	};
}