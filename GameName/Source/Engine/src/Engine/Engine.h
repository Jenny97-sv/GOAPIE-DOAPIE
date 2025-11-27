#pragma once
#include <iostream>
#include <memory>
#include "Graphics/GraphicsEngine.h"

class Engine
{
public:
	static Engine* GetInstance()
	{
		return myIntance;
	}
	static bool StartEngine();

	bool InternalStart(int width, int height, HWND& windowHandle);

	GraphicsEngine& GetGraphicsEngine() { return *myGraphicsEngine; }

private:
	static Engine* myIntance;
	std::unique_ptr<GraphicsEngine> myGraphicsEngine;
};