#pragma once
#include "GraphicsEngineData.h"

#include <wrl/client.h>
#include <d3d11.h>
#include "Camera.h"

#include <array>
#include "FullScreenEffect\FullscreenEffect.h"

#include <memory>
#include "RayTracing\RayTracing.h"

using Microsoft::WRL::ComPtr;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11DepthStencilView;
struct ID3D11SamplerState;
struct ID3D11RasterizerState;
struct ID3D11Buffer;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
struct D3D11_VIEWPORT;

class Sprite;

#include <CommonUtilites\include\InputHandler.h>

#include "GBuffer\GBuffer.h"

#include "RenderTarget.h"

#include "DepthBuffer.h"

#include "Sprite/SpriteDrawer.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	//static GraphicsEngine& GetInstance() {
	//	static GraphicsEngine instance;
	//	return instance;
	//}

	void Shutdown();

	bool Init(int height, int width, HWND windowHandle);

	void Update(float aDeltaTime);

	void NewFrame();
	void Render();
	void EndRender();

	Vector2i& GetWindowSize() { return myWindowSize; } // NOTE(Will): [TO_BOT_MACKAN]: Should this version really exist!? They should not be modified? Or?
	const Vector2i& GetWindowSize() const { return myWindowSize; }

	ComPtr<ID3D11Device>& GetDevice() { return myDevice; }
	ComPtr<ID3D11DeviceContext>& GetContext() { return myContext; }
	ComPtr<ID3D11Buffer>& GetObjectBuffer() { return myObjectBuffer; }

	SpriteDrawer& GetSpriteDrawer() { return *mySpriteDrawer; }

	inline void SetClearColor(const float aRGB[3]) { memcpy_s(&myClearColor, sizeof(myClearColor), aRGB, sizeof(float) * 3); }
	void SetClearColor(float aR, float aG, float aB)
	{
		myClearColor[0] = aR;
		myClearColor[1] = aG;
		myClearColor[2] = aB;
	}
	float* GetClearColor() { return myClearColor; } // Note(Will): Why does this return a pointer and not a refrence? (Wondering if any specific reason?)

	ComPtr<ID3D11BlendState> GetBlendState(BlendState aState) { return myBlendStates[(int)aState]; }
	Camera::Camera GetCamera() { return myCamera; };
	CommonUtilities::InputHandler GetInputHandler() { return myInput; }

private:

	RayTracing* myRayTracer;

	ComPtr<ID3D11ComputeShader> myComputeShader = nullptr;

	//ComPtr<ID3D11Buffer> myVertexBuffer;

	//Vertex myVertex[4];

	//ComPtr<ID3D11VertexShader> myVertexShader;
	//ComPtr<ID3D11PixelShader> myPixelShader;
	//ComPtr<ID3D11InputLayout> myInputLayout;

	std::unique_ptr<SpriteDrawer> mySpriteDrawer;

	ComPtr<ID3D11ShaderResourceView> myCubemap = nullptr;

	unsigned int myNumMips = 0;

	bool CreateSamplers();
	bool CreateBlendStates();
	bool CreateRasterizerStates();
	bool CreateDepthStencilStates();

	ComPtr<ID3D11Device> myDevice;
	ComPtr<ID3D11DeviceContext> myContext;
	ComPtr<IDXGISwapChain> mySwapChain;

	RenderTarget* BackBufferptr = nullptr;
	DepthBuffer* DepthBufferptr = nullptr;
	RenderTarget myBackBuffer;
	DepthBuffer myDepthBuffer;

	std::array<ComPtr<ID3D11BlendState>, (int)BlendState::Count> myBlendStates;
	std::array<ComPtr<ID3D11RasterizerState>, (int)RasterizerState::Count> myRasterizerStates;
	std::array<ComPtr<ID3D11DepthStencilState>, (int)DepthStencilState::Count> myDepthStencilStates;
	std::array<std::array<ComPtr<ID3D11SamplerState>, (int)SamplerAddressMode::Count>, (int)SamplerFilter::Count> mySamplerStates;

	SamplerFilter mySamplerFilter = SamplerFilter::Trilinear;
	SamplerAddressMode mySamplerAddressMode = SamplerAddressMode::Wrap;

	DepthStencilState myDepthStencilState = DepthStencilState::Write;
	BlendState myBlendState = BlendState::Disabled;
	RasterizerState myRasterizerState = RasterizerState::BackfaceCulling;

	float myClearColor[4]{ 0 };

	ComPtr<ID3D11Buffer> myFrameBuffer;
	ComPtr<ID3D11Buffer> myObjectBuffer;
	ComPtr<ID3D11Buffer> myCameraBuffer;
	ComPtr<ID3D11Buffer> myLightBuffer;

	Vector2i myWindowSize;

	Vector2f myViewport;

	std::unique_ptr<FullscreenEffect> myFullScreenDeferredRendering;
	Camera::Camera myCamera;

	CommonUtilities::InputHandler myInput;

	GBuffer myGbuffer;
};

