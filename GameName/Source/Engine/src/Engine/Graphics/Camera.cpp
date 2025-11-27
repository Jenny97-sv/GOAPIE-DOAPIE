#include "Camera.h"
#include <Windows.h>

Camera::Camera::Camera()
{
	myProjectionMatrix = DirectX::XMMatrixIdentity();
	myCameraToWorldMatrix = DirectX::XMMatrixIdentity();
	myFov = 0;
	myNearPlane = 0;
	myFarPlane = 0;
	myHFar = 0;
	myWFar = 0;
	myHNear = 0;
	myWNear = 0;
	myHFoV = 0;
	myVFoV = 0;
	myHeight = 0;
	myWidth = 0;
}


Camera::Camera::~Camera()
{
}


void Camera::Camera::Update(float& aDeltaTime)
{
	POINT currentMousePos;
	GetCursorPos(&currentMousePos);
	int deltaX = prevMousePos.x - currentMousePos.x;
	int deltaY = currentMousePos.y - prevMousePos.y;
	float aSpeed = 150;

	if (GetAsyncKeyState(VK_SHIFT))
	{
		aSpeed *= 5;
	}
	if (GetAsyncKeyState(RI_MOUSE_LEFT_BUTTON_UP))
	{

		myTestRotation.y -= (float)deltaX * aDeltaTime / 2;
		myTestRotation.x += (float)deltaY * aDeltaTime / 2;
	}

	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(myTestRotation.x, myTestRotation.y, myTestRotation.z);

	DirectX::XMVECTOR upDir = DirectX::XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);

	DirectX::XMMATRIX vectorRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, myTestRotation.y, 0.0f);
	myXMVectorForward = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vectorRotationMatrix);
	myXMVectorBack = DirectX::XMVector3TransformCoord(DEFAULT_BACK_VECTOR, vectorRotationMatrix);
	myXMVectorRight = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vectorRotationMatrix);
	myXMVectorLeft = DirectX::XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vectorRotationMatrix);

	// Handle translation based on input ('W' and 'S' keys)
	if (GetAsyncKeyState('W') & 0x8000)
	{
		myPos.x += DirectX::XMVectorGetX(myXMVectorForward) * aSpeed * aDeltaTime;
		myPos.z += DirectX::XMVectorGetZ(myXMVectorForward) * aSpeed * aDeltaTime;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		myPos.x += DirectX::XMVectorGetX(myXMVectorBack) * aSpeed * aDeltaTime;
		myPos.z += DirectX::XMVectorGetZ(myXMVectorBack) * aSpeed * aDeltaTime;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		myPos.x += DirectX::XMVectorGetX(myXMVectorRight) * aSpeed * aDeltaTime;
		myPos.z += DirectX::XMVectorGetZ(myXMVectorRight) * aSpeed * aDeltaTime;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		myPos.x += DirectX::XMVectorGetX(myXMVectorLeft) * aSpeed * aDeltaTime;
		myPos.z += DirectX::XMVectorGetZ(myXMVectorLeft) * aSpeed * aDeltaTime;
	}

	// Move upward when looking up
	if (GetAsyncKeyState('E') & 0x8000)
	{
		myPos.y += aSpeed * aDeltaTime;
	}

	// Move downward when looking down
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		myPos.y -= aSpeed * aDeltaTime;
	}

	// Update the camera matrix
	myCameraToWorldMatrix = DirectX::XMMatrixScaling(1, 1, 1) *
		DirectX::XMMatrixRotationRollPitchYaw(myTestRotation.x, myTestRotation.y, myTestRotation.z) *
		DirectX::XMMatrixTranslation(myPos.x, myPos.y, myPos.z);

	prevMousePos = currentMousePos;
}

void Camera::Camera::SetPerspectiveProjection(const float& aFov, const float& aWidth, const float& aHeight, const float& aNearPlane, const float& aFarPlane)
{
	myFov = aFov * (3.14159265359f / 180.f);
	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;
	myWidth = aWidth;
	myHeight = aHeight;

	myHFoV = aFov * 3.14159265359f / 180.f;
	myVFoV = 2 * std::atan(std::tan(myHFoV / 2) * (aHeight / aWidth));

	myHFar = 2 * tan(myHFoV / 2.f) * aFarPlane;
	myWFar = myHFar * myHFoV / myVFoV;
	myHNear = 2 * tan(myHFoV / 2.f) * aNearPlane;
	myWNear = myHNear * myHFoV / myVFoV;
	float Q = myFarPlane / (myFarPlane - myNearPlane);

	myProjectionMatrix = DirectX::XMMatrixIdentity();

	myProjectionMatrix.r[0].m128_f32[0] = 1 / tan(myHFoV / 2);
	myProjectionMatrix.r[1].m128_f32[1] = (1 / tan(myVFoV / 2));
	myProjectionMatrix.r[2].m128_f32[2] = Q;
	myProjectionMatrix.r[2].m128_f32[3] = 1.f / Q;
	myProjectionMatrix.r[3].m128_f32[2] = -Q * myNearPlane;
	myProjectionMatrix.r[3].m128_f32[3] = 0;
}

void Camera::Camera::SetOrtographicProjection(float aWidth, float aHeight, float aDepth)
{
	myProjectionMatrix = {};

	myProjectionMatrix.r[0].m128_f32[0] = 2.f / aWidth;
	myProjectionMatrix.r[1].m128_f32[1] = 2.f / aHeight;
	myProjectionMatrix.r[2].m128_f32[2] = 1.f / aDepth;
}

void Camera::Camera::SetOrtographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar)
{
	myProjectionMatrix = DirectX::XMMatrixIdentity();

	myProjectionMatrix.r[0].m128_f32[0] = 2.f / (aRight - aLeft);
	myProjectionMatrix.r[1].m128_f32[1] = 2.f / (aBottom - aTop);
	myProjectionMatrix.r[2].m128_f32[2] = 1.f / (aFar - aNear);

	myProjectionMatrix.r[3].m128_f32[0] = -(aRight + aLeft) / (aRight - aLeft);
	myProjectionMatrix.r[3].m128_f32[1] = -(aBottom + aTop) / (aBottom - aTop);
	myProjectionMatrix.r[3].m128_f32[2] = -(aNear) / (aFar - aNear);
}
