#include "RenderTarget.h"
#include "../Engine.h"
#include "DepthBuffer.h"

RenderTarget::RenderTarget()
	:myRenderTarget(nullptr)
	, myViewport(nullptr)
	, mySRV(nullptr)
{}

RenderTarget::~RenderTarget() = default;

RenderTarget RenderTarget::Create(const Vector2i& aSize, DXGI_FORMAT aFormat)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = aSize.x;
	desc.Height = aSize.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = aFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	auto device = Engine::GetInstance()->GetGraphicsEngine().GetDevice();

	ID3D11Texture2D* texture;
	result = device->CreateTexture2D(&desc, nullptr, &texture);

	RenderTarget textureResult = Create(texture);

	ID3D11ShaderResourceView* SRV;
	result = device->CreateShaderResourceView(texture, nullptr, &SRV);

	textureResult.mySRV = SRV;
	SRV->Release();

	return textureResult;
}

RenderTarget RenderTarget::Create(ID3D11Texture2D* aTexture)
{
	HRESULT result;

	ID3D11RenderTargetView* RTV;
	result = Engine::GetInstance()->GetGraphicsEngine().GetDevice()->CreateRenderTargetView(
		aTexture,
		nullptr,
		&RTV);

	RenderTarget textureResult;

	if (aTexture)
	{
		D3D11_TEXTURE2D_DESC desc;
		aTexture->GetDesc(&desc);
		textureResult.myViewport = std::make_shared<const D3D11_VIEWPORT>(D3D11_VIEWPORT{
				0,
				0,
				static_cast<float>(desc.Width),
				static_cast<float>(desc.Height),
				0,
				1
			});
	}

	textureResult.myRenderTarget = RTV;
	RTV->Release();
	return textureResult;
}

void RenderTarget::Clear(float* aClearColor)
{
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->ClearRenderTargetView(myRenderTarget.Get(), aClearColor);
}

void RenderTarget::SetAsActiveTarget(DepthBuffer* aDepth)
{
	auto context = Engine::GetInstance()->GetGraphicsEngine().GetContext();
	if (aDepth)
	{
		context->OMSetRenderTargets(1, myRenderTarget.GetAddressOf(), aDepth->GetDepthStencilView());
	}
	else
	{
		context->OMSetRenderTargets(1, myRenderTarget.GetAddressOf(), nullptr);
	}

	context->RSSetViewports(1, myViewport.get());
}

void RenderTarget::SetAsResourceOnSlot(unsigned int aSlot) const
{
	auto context = Engine::GetInstance()->GetGraphicsEngine().GetContext();
	context->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
}
