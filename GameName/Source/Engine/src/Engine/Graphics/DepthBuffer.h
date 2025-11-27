#pragma once
#include <wrl/client.h>
#include <CommonUtilites\Vector\Vector2.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct D3D11_VIEWPORT;
struct ID3D11ShaderResourceView;

class DepthBuffer
{
private:
	ComPtr<ID3D11DepthStencilView> myDepth = 0;

public:
	D3D11_VIEWPORT myViewport = {};
	ComPtr<ID3D11ShaderResourceView> mySRV;

	static DepthBuffer Create(CommonUtilities::Vector2<int> aSize);

	void Clear(float aClearDepthValue = 1.0f, uint8_t aClearStencilValue = 0u);
	ID3D11DepthStencilView* GetDepthStencilView() { return myDepth.Get(); };

	void SetAsActiveTarget();
	void SetAsResourceOnSlot(const unsigned int aSlot);
};