#include "Engine.h"

Engine* Engine::myIntance = nullptr;

bool Engine::StartEngine()
{
	if (!myIntance)
	{
		myIntance = new Engine();
		return true;
	}
	return false;
}

bool Engine::InternalStart(int width, int height, HWND& windowHandle)
{
	myGraphicsEngine = std::make_unique<GraphicsEngine>();
	if (!myGraphicsEngine->Init(height, width, windowHandle))
	{
		return false;
	}
	return true;
}