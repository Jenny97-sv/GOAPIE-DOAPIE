#pragma once
#include "CommonUtilites\Vector\Vector.h"
#include "DirectXTK\SpriteBatch.h"


class Texture;
class Sprite
{
public:
	Sprite();
	~Sprite();

	bool Init(const std::string& aTexturePath);

	void Draw();

	void SetPosition(const Vector2f& aPosition) { myPosition = aPosition; }
	const Vector2f& GetPosition() const { return myPosition; }
	Vector2f& GetPosition() { return myPosition; }

	void SetScale(const Vector2f& aSize) { myScale = aSize; }
	const Vector2f& GetScale() const { return myScale; }
	Vector2f& GetScale() { return myScale; }

	void SetRect(const RECT& aRect) { myRect = aRect; }
	const RECT& GetRect() const { return myRect; }
	RECT& GetRect() { return myRect; }

	void SetColor(const Vector4f& aColor) { myColor = aColor; }
	const Vector4f& GetColor() const { return myColor; }
	Vector4f& GetColor() { return myColor; }

	void SetTexture(Texture& aTexture) { myTexture = &aTexture; }
	const Texture& GetTexture() const { return *myTexture; }
	Texture& GetTexture() { return *myTexture; }

	void SetRotation(float aRotation) { myRotation = aRotation; }
	const float& GetRotation() const { return myRotation; }
	float& GetRotation() { return myRotation; }

private:
	Vector2f myPosition;
	Vector2f myScale;
	RECT myRect;
	Texture* myTexture;
	Vector4f myColor;
	float myRotation;
};
