#pragma once
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct D3D11_VIEWPORT;

class FullscreenEffect
{
public:
	~FullscreenEffect() = default;
	FullscreenEffect();
	bool Init(const char* aVertexShaderPath, const char* aPixelShaderPath);
	//bool Init(const char* aPixelShaderPath);
	void Render();
	//void RecompileShader(ID3D11Device* aDevice, const wchar_t* aPixelShaderPath);
private:
	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;

};