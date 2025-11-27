#pragma once
#include <CommonUtilites/Vector/Vector.h>
#include <DirectXMath.h>
#include <windows.h>

class Camera
{
public:
	Camera();
	~Camera();

	void Update(float& aDeltaTime);

	void SetPerspectiveProjection(
		const float& aFov,
		const float& aWidth,
		const float& aHeight,
		const float& aNearPlane,
		const float& aFarPlane
	);

	void SetOrtographicProjection(float aWidth, float aHeight, float aDepth);
	void SetOrtographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar);

	DirectX::XMFLOAT3& GetPos() { return myPos; }

	inline Vector3f GetPosAsVector3f() { return Vector3f(myPos.x, myPos.y, myPos.z); }

	float& GetNearPlane() { return myNearPlane; }
	float& GetFarPlane() { return myFarPlane; }

	DirectX::XMMATRIX& GetCameraToWorldMatrix() { return myCameraToWorldMatrix; }
	DirectX::XMMATRIX GetWorldToClipMatrix() { return XMMatrixInverse(nullptr, myCameraToWorldMatrix); }

	DirectX::XMMATRIX& GetProjectionMatrix() { return myProjectionMatrix; }

	Vector3f GetForward() const
	{
		return Vector3f(myCameraToWorldMatrix.r[2].m128_f32[0], myCameraToWorldMatrix.r[2].m128_f32[1], myCameraToWorldMatrix.r[2].m128_f32[2]).GetNormalized();
	}

private:

	Vector2f myResolution;

	DirectX::XMMATRIX myCameraToWorldMatrix = {};
	DirectX::XMMATRIX myProjectionMatrix = {};

	const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
	const DirectX::XMVECTOR DEFAULT_BACK_VECTOR = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
	const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
	const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);

	DirectX::XMVECTOR myXMVectorForward;
	DirectX::XMVECTOR myXMVectorRight;
	DirectX::XMVECTOR myXMVectorLeft;
	DirectX::XMVECTOR myXMVectorBack;

	DirectX::XMFLOAT3 myPos;

	POINT prevMousePos;

	Vector3f myTestRotation;

	float myFov = 90.f;
	float myHFoV;
	float myVFoV;

	float myRotation = 0.f;
	float myNearPlane = 0.01f;
	float myFarPlane = 1500.0f;

	float myWidth;
	float myHeight;

	float myHFar;
	float myWFar;
	float myHNear;
	float myWNear;
};
