#include "GBuffer.h"
#include "../DepthBuffer.h"
#include "../Engine/src/Engine/Engine.h" 

GBuffer::GBuffer()
{
}

GBuffer::~GBuffer()
{
}

GBuffer GBuffer::Create(Vector2i aSize)
{
	auto& device = Engine::GetInstance()->GetGraphicsEngine().GetDevice();
	HRESULT result;
	std::array<DXGI_FORMAT, (int)GBufferTexture::Count> textureFormats =
	{
	DXGI_FORMAT_R32G32B32A32_FLOAT,// Position
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,// Albedo
	DXGI_FORMAT_R10G10B10A2_UNORM,// Normal,
	DXGI_FORMAT_R8G8B8A8_UNORM,// Material
	DXGI_FORMAT_R8G8B8A8_UNORM// EffectsAndCustom
	};

	GBuffer returnGBuffer;
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = aSize.x;
	desc.Height = aSize.y;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	returnGBuffer.myViewport = std::make_shared<D3D11_VIEWPORT>(
		D3D11_VIEWPORT{
		0,
		0,
		static_cast<float>(desc.Width),
		static_cast<float>(desc.Height),
		0,
		1
		});

	for (unsigned int idx = 0; idx < (int)GBufferTexture::Count; idx++)
	{
		desc.Format = textureFormats[idx];
		result = device->CreateTexture2D(&desc, nullptr, &returnGBuffer.myTextures[idx]);

		result = device->CreateRenderTargetView(
			returnGBuffer.myTextures[idx].Get(),
			nullptr,
			returnGBuffer.myRTVs[idx].GetAddressOf());

		result = device->CreateShaderResourceView(
			returnGBuffer.myTextures[idx].Get(),
			nullptr,
			returnGBuffer.mySRVs[idx].GetAddressOf());

	}

	return returnGBuffer;
}

void GBuffer::ClearTextures()
{
	GraphicsEngine& graphicsEngine = Engine::GetInstance()->GetGraphicsEngine();
	auto context = graphicsEngine.GetContext();
	for (unsigned int idx = 0; idx < (int)GBufferTexture::Count; idx++)
	{
		float shit[4] = {};
		context->ClearRenderTargetView(myRTVs[idx].Get(), shit);
	}

	//context->ClearRenderTargetView(myRTVs[(int)GBufferTexture::Albedo].Get(), shit);
}

void GBuffer::SetAsActiveTarget(DepthBuffer* aDepthBuffer)
{
	auto context = Engine::GetInstance()->GetGraphicsEngine().GetContext();
	if (aDepthBuffer)
	{
		context->OMSetRenderTargets((int)GBufferTexture::Count, myRTVs[0].GetAddressOf(), aDepthBuffer->GetDepthStencilView());
	}
	else
	{
		context->OMSetRenderTargets((int)GBufferTexture::Count, myRTVs[0].GetAddressOf(), nullptr);
	}
	context->RSSetViewports(1, myViewport.get());
}

void GBuffer::SetAsResourceOnSlot(GBufferTexture aTexture, unsigned int aSlot)
{
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->PSSetShaderResources(aSlot, 1, mySRVs[(int)aTexture].GetAddressOf());
}

void GBuffer::SetAllAsResources(unsigned aSlot)
{
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->PSSetShaderResources(aSlot, (int)GBufferTexture::Count, mySRVs[0].GetAddressOf());
}

void GBuffer::ClearAllAsResources(unsigned int aSlot)
{
	ID3D11ShaderResourceView* const nullSRV[6] = { NULL };
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->PSSetShaderResources(aSlot, (int)GBufferTexture::Count, nullSRV);
}
