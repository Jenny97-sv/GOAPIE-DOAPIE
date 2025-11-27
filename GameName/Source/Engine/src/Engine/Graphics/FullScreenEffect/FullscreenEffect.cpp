#include "FullscreenEffect.h"
#include "../Engine/src/Engine/Engine.h"
#include <fstream>
#include <d3dcompiler.h>

FullscreenEffect::FullscreenEffect()
{
}

bool FullscreenEffect::Init(const char* aVertexShaderPath, const char* aPixelShaderPath)
{
	HRESULT result;
	auto device = Engine::GetInstance()->GetGraphicsEngine().GetDevice();
	std::string vsData;
	// Load shaders
	//std::string shaderPath = STATIC_SHADER_PATH;
	std::ifstream vsFile;
	vsFile.open(aVertexShaderPath, std::ios::binary);
	//vsFile.open("Fullscreen_VS.cso", std::ios::binary);

	vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	result = device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &myVertexShader);
	if (FAILED(result))
	{
		return false;
	}
	vsFile.close();
	std::ifstream psFile;
	psFile.open(aPixelShaderPath, std::ios::binary);

	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	result = device->CreatePixelShader(psData.data(), psData.size(), nullptr, &myPixelShader);
	if (FAILED(result))
	{
		return false;
	}
	psFile.close();

	return true;
}

void FullscreenEffect::Render()
{
	auto context = Engine::GetInstance()->GetGraphicsEngine().GetContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(nullptr);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	context->VSSetShader(myVertexShader.Get(), nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(myPixelShader.Get(), nullptr, 0);
	context->Draw(3, 0);
}