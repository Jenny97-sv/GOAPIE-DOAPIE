#include "DepthBuffer.h"
#include "../Engine.h"

void DepthBuffer::SetAsActiveTarget()
{
	auto& context = Engine::GetInstance()->GetGraphicsEngine().GetContext();
	context->OMSetRenderTargets(0, nullptr, GetDepthStencilView());
	context->RSSetViewports(1, &myViewport);
}

void DepthBuffer::SetAsResourceOnSlot(const unsigned int aSlot)
{
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}

void DepthBuffer::Clear(float aClearDepthValue /* = 1.0f */, uint8_t aClearStencilValue /* = 0 */)
{
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->ClearDepthStencilView(myDepth.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, aClearDepthValue, aClearStencilValue);
}

DepthBuffer DepthBuffer::Create(CommonUtilities::Vector2<int> aSize)
{
	auto& graphicsEngine = Engine::GetInstance()->GetGraphicsEngine();

	HRESULT result;
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = aSize.x;
	desc.Height = aSize.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* texture;
	result = graphicsEngine.GetDevice()->CreateTexture2D(&desc, nullptr, &texture);


	ID3D11DepthStencilView* DSV;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	CD3D11_DEPTH_STENCIL_DESC  test = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
	test.StencilEnable = true;
	test.StencilWriteMask = 0xFF;

	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	result = graphicsEngine.GetDevice()->CreateDepthStencilView(texture, &dsvDesc, &DSV);


	DepthBuffer textureResult;
	textureResult.myDepth = DSV;
	DSV->Release();

	ID3D11ShaderResourceView* SRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc{};
	srDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = std::numeric_limits<UINT>::max();
	result = graphicsEngine.GetDevice()->CreateShaderResourceView(texture, &srDesc, &SRV);

	textureResult.mySRV = SRV;
	SRV->Release();

	textureResult.myViewport = {
			0,
			0,
			static_cast<float>(aSize.x),
			static_cast<float>(aSize.y),
			0,
			1
	};
	return textureResult;
}
