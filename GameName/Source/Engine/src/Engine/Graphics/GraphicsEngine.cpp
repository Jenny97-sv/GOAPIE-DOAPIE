#include "GraphicsEngine.h"
#include "Sprite/Sprite.h"
#include <DirectXMath.h>
#define ENABLE_PHYSX_MANAGER

//#include <filesystem>

#include "DDSTextureLoader\DDSTextureLoader11.h"
#include <limits>

#include <random>
#include <fstream>

GraphicsEngine::GraphicsEngine()
{

}

GraphicsEngine::~GraphicsEngine()
{
}

void GraphicsEngine::Shutdown()
{


}

bool GraphicsEngine::Init(int width, int height, HWND windowHandle)
{
	myWindowSize.x = width;
	myWindowSize.y = height;

	myCamera.SetPerspectiveProjection(90, (float)myWindowSize.x, (float)myWindowSize.y, 0.01f, 10000.f);

	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = true;
	UINT creationFlags = 0;

#if defined(REPORT_DX_WARNINGS)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapChain,
		&myDevice,
		nullptr,
		&myContext
	);

	{
		D3D11_BUFFER_DESC bufferDescription = { 0 };
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDescription.ByteWidth = sizeof(FrameBufferData);
		result = myDevice->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer);
		if (FAILED(result) && "FramBuffer Failed")
		{
			return false;
		}
		bufferDescription.ByteWidth = sizeof(ObjectBufferData);
		result = myDevice->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
		if (FAILED(result) && "myObjectBuffer Failed")
		{
			return false;
		}
		result = myDevice->CreateBuffer(&bufferDescription, nullptr, &myCameraBuffer);
		if (FAILED(result) && "myCamerabuffer Failed")
		{
			return false;
		}
		result = myDevice->CreateBuffer(&bufferDescription, NULL, myLightBuffer.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			return false;
		}
	}

	ID3D11Texture2D* backBufferTexture;
	result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
	if (FAILED(result))
	{
		return false;
	}
	myBackBuffer = RenderTarget::Create(backBufferTexture);
	BackBufferptr = &myBackBuffer;


	D3D11_TEXTURE2D_DESC textureDesc;
	backBufferTexture->GetDesc(&textureDesc);
	backBufferTexture->Release();

	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(textureDesc.Width);
	viewport.Height = static_cast<float>(textureDesc.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);

	{
		ID3D11Texture2D* depthBufferTexture;
		D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
		depthBufferDesc.Width = static_cast<unsigned int>(textureDesc.Width);
		depthBufferDesc.Height = static_cast<unsigned int>(textureDesc.Height);
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		result = myDevice->CreateTexture2D(&depthBufferDesc, nullptr, &depthBufferTexture);
		if (FAILED(result) && "depthBuffer Failed")
		{
			return false;
		}
		myDepthBuffer = DepthBuffer::Create(Vector2i((int)textureDesc.Width, (int)textureDesc.Height));
		DepthBufferptr = &myDepthBuffer;
		depthBufferTexture->Release();
	}

	mySpriteDrawer = std::make_unique<SpriteDrawer>();
	mySpriteDrawer->Init(myContext);


	myBackBuffer.SetAsActiveTarget(&myDepthBuffer);

	myFullScreenDeferredRendering = std::make_unique<FullscreenEffect>();

	myViewport = CommonUtilities::Vector2<float>((float)textureDesc.Width, (float)textureDesc.Height);

	if (!CreateSamplers())
	{
	}
	if (!CreateBlendStates())
	{
	}
	if (!CreateRasterizerStates())
	{
	}
	if (!CreateDepthStencilStates())
	{
	}

	myContext->OMSetBlendState(myBlendStates[(int)myBlendState].Get(), nullptr, 0xffffffff);

	myContext->OMSetDepthStencilState(myDepthStencilStates[(int)myDepthStencilState].Get(), 0);

	myContext->RSSetState(myRasterizerStates[(int)myRasterizerState].Get());

	myContext->PSSetSamplers(0, 1, mySamplerStates[(int)mySamplerFilter][(int)mySamplerAddressMode].GetAddressOf());

	myGbuffer = GBuffer::Create(Vector2i(Vector2i((int)textureDesc.Width, (int)textureDesc.Height)));

	myRayTracer = new RayTracing();
	myRayTracer->Init();

	return true;
}

void GraphicsEngine::Update(float aDeltaTime)
{
	//myDeltaTime.Update();
	//float deltaTime = myDeltaTime.GetDeltaTime();
	//std::cout << 1.f / myDeltaTime.GetDeltaTime() << std::endl;
	myInput.Update();
	myCamera.Update(aDeltaTime);
}

void GraphicsEngine::NewFrame()
{
	BackBufferptr->SetAsActiveTarget(DepthBufferptr);
	DepthBufferptr->Clear(1.0f, 0);
	BackBufferptr->Clear(myClearColor);

	myContext->PSSetSamplers(0, 1, mySamplerStates[(int)mySamplerFilter][(int)mySamplerAddressMode].GetAddressOf());
	//mySpotComponent->GetSpotLight().GetTransform().RotateAroundAxis({ 0,1,0 }, 90 * myDeltaTime.GetDeltaTime());
	myContext->OMSetBlendState(myBlendStates[(int)myBlendState].Get(), nullptr, 0xffffffff);

	myContext->OMSetDepthStencilState(myDepthStencilStates[(int)myDepthStencilState].Get(), 0);

	myContext->RSSetState(myRasterizerStates[(int)myRasterizerState].Get());
}

void GraphicsEngine::Render()
{
	myRayTracer->Render();
	/*myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myContext->IASetInputLayout(myInputLayout.Get());
	myContext->PSSetShader(myPixelShader.Get(), nullptr, 0);
	myContext->VSSetShader(myVertexShader.Get(), nullptr, 0);


	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	myContext->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &stride, &offset);


	myContext->Draw(4, 0);*/

	//myGbuffer.SetAllAsResources(10);

	//myBackBuffer.SetAsActiveTarget(nullptr);

	//myContext->OMSetBlendState(myBlendStates[(int)BlendState::AdditiveBlend].Get(), nullptr, 0xffffffff);
	//myContext->RSSetState(myRasterizerStates[(int)RasterizerState::FrontCulling].Get());

	//EndRender();
}

void GraphicsEngine::EndRender()
{
	mySwapChain->Present(0, 0);
}

bool GraphicsEngine::CreateSamplers()
{
	HRESULT result = S_OK;
	D3D11_SAMPLER_DESC samplerDesc = {};

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Point][(int)SamplerAddressMode::Clamp].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Point][(int)SamplerAddressMode::Wrap].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Point][(int)SamplerAddressMode::Mirror].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Bilinear][(int)SamplerAddressMode::Clamp].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Bilinear][(int)SamplerAddressMode::Wrap].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Bilinear][(int)SamplerAddressMode::Mirror].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Trilinear][(int)SamplerAddressMode::Clamp].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Trilinear][(int)SamplerAddressMode::Wrap].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = myDevice->CreateSamplerState(&samplerDesc, mySamplerStates[(int)SamplerFilter::Trilinear][(int)SamplerAddressMode::Mirror].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsEngine::CreateBlendStates()
{
	HRESULT result = S_OK;
	D3D11_BLEND_DESC blendStateDescription = {};

	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = myDevice->CreateBlendState(&blendStateDescription, myBlendStates[(int)BlendState::Disabled].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	blendStateDescription = {};
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = myDevice->CreateBlendState(&blendStateDescription, myBlendStates[(int)BlendState::AlphaBlend].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	blendStateDescription = {};
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = myDevice->CreateBlendState(&blendStateDescription, myBlendStates[(int)BlendState::AdditiveBlend].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	blendStateDescription = {};
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;
	result = myDevice->CreateBlendState(&blendStateDescription, myBlendStates[(int)BlendState::AddAndFadeBackground].ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	return true;
}

bool GraphicsEngine::CreateDepthStencilStates()
{
	HRESULT result = S_OK;

	D3D11_DEPTH_STENCIL_DESC readOnlyDepthDesc = {};
	readOnlyDepthDesc.DepthEnable = true;
	readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	readOnlyDepthDesc.StencilEnable = false;

	result = myDevice->CreateDepthStencilState(&readOnlyDepthDesc, &myDepthStencilStates[(int)DepthStencilState::ReadOnly]);
	if (FAILED(result))
		return false;

	readOnlyDepthDesc = {};
	readOnlyDepthDesc.DepthEnable = true;
	readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	readOnlyDepthDesc.StencilEnable = false;
	result = myDevice->CreateDepthStencilState(&readOnlyDepthDesc,
		&myDepthStencilStates[(int)DepthStencilState::ReadOnlyGreater]);
	if (FAILED(result))
		return false;

	readOnlyDepthDesc = {};
	readOnlyDepthDesc.DepthEnable = true;
	readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	readOnlyDepthDesc.StencilEnable = false;
	result = myDevice->CreateDepthStencilState(&readOnlyDepthDesc,
		&myDepthStencilStates[(int)DepthStencilState::ReadOnlyLess]);
	if (FAILED(result))
		return false;

	readOnlyDepthDesc = {};
	readOnlyDepthDesc.DepthEnable = true;
	readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	readOnlyDepthDesc.StencilEnable = false;
	result = myDevice->CreateDepthStencilState(&readOnlyDepthDesc,
		&myDepthStencilStates[(int)DepthStencilState::ReadGreaterEqual]);
	if (FAILED(result))
		return false;


	myDepthStencilStates[(int)DepthStencilState::Write] = nullptr;

	return true;
}

bool GraphicsEngine::CreateRasterizerStates()
{
	HRESULT result = S_OK;

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = true;

	result = myDevice->CreateRasterizerState(&rasterizerDesc, &myRasterizerStates[(int)RasterizerState::Wireframe]);
	if (FAILED(result))
		return false;

	rasterizerDesc.CullMode = D3D11_CULL_NONE;

	result = myDevice->CreateRasterizerState(&rasterizerDesc, &myRasterizerStates[(int)RasterizerState::WireframeNoCulling]);
	if (FAILED(result))
		return false;

	rasterizerDesc = {};
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = myDevice->CreateRasterizerState(&rasterizerDesc, &myRasterizerStates[(int)RasterizerState::NoFaceCulling]);
	if (FAILED(result))
		return false;

	rasterizerDesc = {};
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	result = myDevice->CreateRasterizerState(&rasterizerDesc, &myRasterizerStates[(int)RasterizerState::FrontfaceCulling]);

	result = myDevice->CreateRasterizerState(&rasterizerDesc, &myRasterizerStates[(int)RasterizerState::FrontCulling]);
	if (FAILED(result))
		return false;

	myRasterizerStates[(int)RasterizerState::BackfaceCulling] = nullptr;

	return true;
}