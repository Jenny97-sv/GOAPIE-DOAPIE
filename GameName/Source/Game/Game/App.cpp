#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <string>
#include <stringapiset.h>
#include "Engine.h"
#include <CommonUtilites/include/Timer.h>
#include "GameWorld.h"
#include "SceneManager.h"
#include "CommonUtilites/include/InputHandler.h"
#include "PostMaster.h"
//#include "BoidMath.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void InitConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 6031 )

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);


	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

#pragma warning( pop )

}

void CloseConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 6031 )
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
}


int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int nCmdShow
) {

	InitConsole();
	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszClassName = L"MaktusEngine";
	RegisterClassExW(&wcex);

	int width = 1280; // RESOLUTION HERE!!!!!
	int height = 720; // RESOLUTION HERE!!!!!
	//BoidMath::resolution = { width, height };

	HWND hWnd = CreateWindow(L"MaktusEngine", L"MaktusEngine",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return 0;
	}

	if (!Engine::StartEngine())
	{
		return 0;
	}
	if (!Engine::GetInstance()->InternalStart(width, height, hWnd))
	{
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	PostMaster::CreateInstance();

	SceneManager& sceneMgr = SceneManager::GetInstance();
	sceneMgr.RegisterScene("Main", std::make_unique<GameWorld>());
	if (!sceneMgr.LoadScene("Main"))
		return 0;

	//GameWorld gameWorld;
	//if (!gameWorld.Init())
	//{
	//	std::cout << "Shit!" << std::endl;
	//	return 0;
	//}

	CommonUtilities::Timer myDeltaTime;

	float fpsTimer = 0.f;
	int frameCounter = 0;


	MSG msg = {};
	bool shouldRun = true;
	while (shouldRun)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				shouldRun = false;
			}
		}
		auto& engine = *Engine::GetInstance();
		myDeltaTime.Update();
		float dt = myDeltaTime.GetDeltaTime();
		fpsTimer += dt;
		frameCounter++;

		if (fpsTimer >= 1.0f)
		{
			float fps = frameCounter / fpsTimer;
			std::cout << "FPS: " << fps << std::endl;

			fpsTimer = 0.f;
			frameCounter = 0;
		}


		engine.GetGraphicsEngine().NewFrame();
		engine.GetGraphicsEngine().Update(myDeltaTime.GetDeltaTime());

		sceneMgr.Update(myDeltaTime.GetDeltaTime());
		sceneMgr.Render();
		
		engine.GetGraphicsEngine().Render();

		engine.GetGraphicsEngine().EndRender();

		if (!sceneMgr.shouldRun)
		{
			shouldRun = false;
		}
	}
	sceneMgr.UnloadCurrentScene();

	PostMaster::DestroyInstance();

	CloseConsole();
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Engine::GetInstance()->GetGraphicsEngine().GetInputHandler().UpdateEvents(message, wParam, lParam);
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}