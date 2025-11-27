#pragma once
#include <wrl/client.h>
#include <DirectXMath.h>
#include "CommonUtilites\Vector\Vector.h"

using Microsoft::WRL::ComPtr;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;


struct Vertex
{
	Vector4f pos;
	Vector4f color;
};

struct FrameBufferData
{
	Vector4f resolution; //myResolution.x = screen width, myResolution.y = screen height, myResolution.z = unset, myResolution.w = unset
	DirectX::XMMATRIX worldToClipMatrix;
	DirectX::XMMATRIX cameraToProjection;
};

struct ObjectBufferData
{
	DirectX::XMMATRIX modelToWorld;
	Vector4f color;
};

struct CameraBufferData
{
	Vector4f cameraPosition;
	Vector3f cameraForward;
	Vector3f cameraRight;
	Vector3f cameraUp;
	float nearPlane;
	float farPlane;
	float time;
	float time123;
	float time1234;
};

struct LightBufferData
{
	/*struct PointLightData
	{
		Vector4f myPosition;
		Vector4f myColorAndIntensity;
		float myRange;
		Vector3f padding;
	} myPointLights[NUMBER_OF_LIGHTS_ALLOWED];

	struct SpotLightData
	{
		Vector4f myPosition;
		Vector4f myColorAndIntensity;
		Vector4f myDirection;
		float myRange;
		float myOuterLimit;
		float myInnerLimit;
		float padding;
	} mySpotLights[NUMBER_OF_LIGHTS_ALLOWED];

	unsigned int myNumberOfPointLights;
	unsigned int myNumberOfSpotLights;
	Vector2f padding0;*/

	DirectX::XMMATRIX directionLightProjection;
	Vector4f dLightPosition;
	Vector4f dLightColorAndIntensity;
	Vector3f dLightDir;
	float padding1;
};

struct VFXBufferData
{
	float vfxTime;
	float vfxIntencity;
	Vector2f padding0;
};

enum class DepthStencilState
{
	Write,
	ReadOnly,
	ReadOnlyGreater,
	ReadOnlyLess,
	ReadGreaterEqual,
	Disabled,
	Count
};

enum class BlendState
{
	Disabled,
	AlphaBlend,
	AdditiveBlend,
	AddAndFadeBackground,
	Count
};

enum class RasterizerState
{
	BackfaceCulling,
	FrontCulling,
	NoFaceCulling,
	Wireframe,
	WireframeNoCulling,
	FrontfaceCulling,
	Count,
};

enum class SamplerFilter
{
	Point,
	Bilinear,
	Trilinear,
	Count
};

enum class SamplerAddressMode
{
	Clamp,
	Wrap,
	Mirror,
	Count
};