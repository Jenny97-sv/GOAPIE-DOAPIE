#pragma once
#include <wrl/client.h>
#include <string>

using Microsoft::WRL::ComPtr;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
class GraphicsEngine;

enum class TextureInitializationType
{
	DDS,
	PNG
};

class Texture
{

public:
	Texture();
	Texture(ID3D11ShaderResourceView* aSRV);
	~Texture();
	bool Initialize(const TextureInitializationType& aType, std::string aPath);// , bool aDoRelative = true);
	void Bind(const int slot, ID3D11DeviceContext* context);

	const int& GetWidth() const { return myWidth; }
	const int& GetHeight() const { return myHeight; }

	ID3D11Texture2D* GetTexture() { return myTexture; };
	const ID3D11Texture2D* GetTexture() const { return myTexture; };
	ID3D11ShaderResourceView* GetShaderResourceView() { return myShaderResourceView.Get(); };
	const ID3D11ShaderResourceView* GetShaderResourceView() const { return myShaderResourceView.Get(); };

	void SetShaderResourceView(ID3D11ShaderResourceView* aSRV);

	void SetTexture(ID3D11Texture2D& aTexture);

	ComPtr<ID3D11ShaderResourceView> GetView();

private:
	bool InitializeDDS(const std::wstring& aPath);

	int myWidth, myHeight;
	unsigned char* myrgbaPixels = nullptr;
	bool myHasInitialized = false;
	ID3D11Texture2D* myTexture = nullptr;
	ComPtr<ID3D11ShaderResourceView> myShaderResourceView;

	ComPtr<ID3D11Device> myDevice;
	ComPtr<ID3D11DeviceContext> myContext;
};
