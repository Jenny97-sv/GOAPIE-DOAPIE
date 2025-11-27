#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include "CommonUtilites\Matrix\Matrix4x4.h"
#include "CommonUtilites\Vector\Vector.h"
#include "../GraphicsEngineData.h"

using Microsoft::WRL::ComPtr;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11DepthStencilView;
struct ID3D11SamplerState;
struct ID3D11RasterizerState;
struct ID3D11Buffer;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;
struct D3D11_VIEWPORT;

struct Sphere
{
	Matrix4x4f world;
	float radius;
	Vector3f position;
	float t;
	Vector3f color;
	bool isLightSource;
};

struct StructuredBuffer
{
	ComPtr<ID3D11Buffer> buffer;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11UnorderedAccessView> uav;
	int element_max_count; // in elements
	int element_size; // in bytes
	//ID3D11Buffer* stagingBuffer;
};

class RayTracing
{
public:
	RayTracing()
	{

	}
	~RayTracing()
	{

	}

	void Init();

	void Update();

	void Render();



private:
	ComPtr<ID3D11Buffer> myVertexBuffer;

	Vertex myVertex[4];

	StructuredBuffer myStructuredBufferOld;
	StructuredBuffer myStructuredBufferNext;

	ComPtr<ID3D11VertexShader> myVertexShader;
	ComPtr<ID3D11PixelShader> myPixelShader;
	ComPtr<ID3D11ComputeShader> myComputeShader;
	ComPtr<ID3D11InputLayout> myInputLayout;

};