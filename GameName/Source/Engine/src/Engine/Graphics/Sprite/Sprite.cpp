#include "Sprite.h"
#include "../Engine/src/Engine/Engine.h"
#include "../Texture.h"
#include "DirectXTK\SpriteBatch.h"

Sprite::Sprite()
	: myPosition(0.0f, 0.0f)
	, myScale(0.0f, 0.0f)
	, myRect{}
	, myTexture(nullptr)
	, myColor{}
	, myRotation(0.0f)
{
}

Sprite::~Sprite()
{
	if (myTexture)
	{
		delete myTexture;
		myTexture = nullptr;
	}
}

bool Sprite::Init(const std::string& aTexturePath)
{
	myTexture = new Texture();

	if (!myTexture->Initialize(TextureInitializationType::DDS, aTexturePath))
	{
		return false;
	}
	myRect = { 0,0, myTexture->GetWidth() + myRect.left,myTexture->GetHeight() + myRect.top };
	Vector2i tempGraphics = Engine::GetInstance()->GetGraphicsEngine().GetWindowSize();
	myPosition = { (float)tempGraphics.x / 2,(float)tempGraphics.y / 2 };
	myScale = { 1,1 };
	myColor = { 1,1,1,1 };

	return true;
}

void Sprite::Draw()
{
	Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().DrawSprite(*this);
}
