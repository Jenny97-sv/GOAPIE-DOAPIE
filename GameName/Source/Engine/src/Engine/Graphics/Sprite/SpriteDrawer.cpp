#include "SpriteDrawer.h"
#include "../Engine/src/Engine/Engine.h"
#include <d3d11.h>
#include "Sprite.h"
#include "../Texture.h"


SpriteDrawer::SpriteDrawer() = default;

SpriteDrawer::~SpriteDrawer() = default;

bool SpriteDrawer::Init(ComPtr<ID3D11DeviceContext> aContext)
{
	mySpriteBatch = std::make_unique<DirectX::SpriteBatch>(aContext.Get());
	if (mySpriteBatch == nullptr)
		return false;

	return true;
}

void SpriteDrawer::DrawSprite(Sprite& aSprite)
{
	mySpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, Engine::GetInstance()->GetGraphicsEngine().GetBlendState(BlendState::AlphaBlend).Get());

	DirectX::XMFLOAT2 tempSpritePos = { aSprite.GetPosition().x,aSprite.GetPosition().y };
	DirectX::FXMVECTOR tempColor = { aSprite.GetColor().x,aSprite.GetColor().y,aSprite.GetColor().z,aSprite.GetColor().w };

	DirectX::XMFLOAT2 tempOrigin = { static_cast<float>(aSprite.GetRect().right + aSprite.GetRect().left) / 2 - (float)aSprite.GetRect().left , static_cast<float>(aSprite.GetRect().bottom + aSprite.GetRect().top) / 2 - (float)aSprite.GetRect().top };
	DirectX::XMFLOAT2 tempScale = { aSprite.GetScale().x,aSprite.GetScale().y };

	mySpriteBatch->Draw(aSprite.GetTexture().GetShaderResourceView(), tempSpritePos, &aSprite.GetRect(), tempColor, aSprite.GetRotation(), tempOrigin, tempScale, DirectX::SpriteEffects_None, 0.f);

	mySpriteBatch->End();

}

