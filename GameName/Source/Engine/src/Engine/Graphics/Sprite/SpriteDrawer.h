#pragma once
#include <DirectXTK\SpriteBatch.h>
#include <wrl\client.h>
#include <vector>

using Microsoft::WRL::ComPtr;
struct ID3D11Device;
struct ID3D11DeviceContext;

#include <memory>

class Sprite;

class SpriteDrawer
{
public:
	SpriteDrawer();
	~SpriteDrawer();

	bool Init(ComPtr<ID3D11DeviceContext> aContext);

	void DrawSprite(Sprite& aSprite);

	void DrawSpriteBatch(const std::vector<Sprite>& someSpritesToDraw);

private:
	std::unique_ptr<DirectX::SpriteBatch> mySpriteBatch;
};

