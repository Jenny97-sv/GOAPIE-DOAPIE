#include "Texture.h"
#include "DDSTextureLoader\DDSTextureLoader11.h"
#include "../Engine.h"
#include "StringHelper\StringHelper.h"


Texture::Texture()
	: myWidth(0), myHeight(0)
	, myShaderResourceView(nullptr)
	, myTexture(nullptr)
{
}

Texture::Texture(ID3D11ShaderResourceView* aSRV)
	: myWidth(0), myHeight(0)
	, myShaderResourceView(aSRV)
	, myTexture(nullptr)
{
}

Texture::~Texture()
{
	if (myrgbaPixels)
	{
		delete myrgbaPixels;
		myrgbaPixels = nullptr;
	}
	if (myTexture)
	{
		myTexture->Release(); // NOTE(Will): Should this not also delete after? (I know it might not be needed, but still, just wondering?)
		myTexture = nullptr;
	}
	// NOTE(Will): Should we not release the SRV!?!?
};

bool Texture::Initialize(const TextureInitializationType& aType, std::string aPath/*, bool aDoRelative*/)
{
	myDevice = Engine::GetInstance()->GetGraphicsEngine().GetDevice();
	myContext = Engine::GetInstance()->GetGraphicsEngine().GetContext();

	std::string path = aPath;
	aType;
	aPath;
	myHasInitialized = true;
	switch (aType)
	{
	case TextureInitializationType::DDS:
		return InitializeDDS(StringHelper::s2ws(aPath));
	default:
		break;
	}
	return false;
}

void Texture::Bind(const int slot, ID3D11DeviceContext* context)
{
	if (!myHasInitialized) return;

	context->PSSetShaderResources(slot, 1, myShaderResourceView.GetAddressOf());
}

void Texture::SetShaderResourceView(ID3D11ShaderResourceView* aSRV)
{
	myShaderResourceView = aSRV;
}

void Texture::SetTexture(ID3D11Texture2D& aTexture)
{
	*myTexture = aTexture;

	D3D11_TEXTURE2D_DESC textureDesc;
	myShaderResourceView->GetResource(reinterpret_cast<ID3D11Resource**>(&myTexture));
	myTexture->GetDesc(&textureDesc);

	myWidth = textureDesc.Width;
	myHeight = textureDesc.Height;

	myHasInitialized = true;
}

ComPtr<ID3D11ShaderResourceView> Texture::GetView()
{
	return myShaderResourceView;
}

bool Texture::InitializeDDS(const std::wstring& aPath)
{
	auto device = myDevice;
	HRESULT result = DirectX::CreateDDSTextureFromFile(
		myDevice.Get(),
		aPath.c_str(),
		nullptr,
		&myShaderResourceView
	);

	if (!FAILED(result))
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		myShaderResourceView->GetResource(reinterpret_cast<ID3D11Resource**>(&myTexture));
		myTexture->GetDesc(&textureDesc);

		myWidth = textureDesc.Width;
		myHeight = textureDesc.Height;
	}

	return !FAILED(result);
}
