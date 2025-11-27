#include "RayTracing.h"
#include <iostream>
#include <fstream>
#include "../Engine/src/Engine/Engine.h"

void RayTracing::Init()
{
	//RayTracerTeste_VS.hlsl
	std::string vsData;

	std::ifstream vsFile;

	std::wstring FUCKOFF = L"../../BinDebug-windows-x86_64/Engine/RayTracerTest_VS.cso";

	vsFile.open(FUCKOFF, std::ios::binary);
	vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	HRESULT result = Engine::GetInstance()->GetGraphicsEngine().GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, myVertexShader.GetAddressOf());

	if (FAILED(result))
	{
		std::cout << "Failed to create" << std::endl;
	}

	vsFile.close();

	std::ifstream psFile;
	psFile.open(L"../../BinDebug-windows-x86_64/Engine/RayTracerTest_PS.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	result = Engine::GetInstance()->GetGraphicsEngine().GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, myPixelShader.GetAddressOf());

	if (FAILED(result))
	{
		return;
	}

	psFile.close();

	std::ifstream cs_File;
	cs_File.open(L"../../BinDebug-windows-x86_64/Engine/RayTracing_CS.cso");
	std::string cs_Data = { std::istreambuf_iterator<char>(cs_File), std::istreambuf_iterator<char>() };
	result = Engine::GetInstance()->GetGraphicsEngine().GetDevice()->CreateComputeShader(cs_Data.data(), cs_Data.size(), nullptr, myComputeShader.GetAddressOf());

	if (FAILED(result))
	{
		return;
	}

	cs_File.close();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	int numElements = ARRAYSIZE(layout);

	result = Engine::GetInstance()->GetGraphicsEngine().GetDevice()->CreateInputLayout(layout, numElements, vsData.data(), vsData.size(), &myInputLayout);
	if (FAILED(result))
	{
		return;
	}

	myVertex[0].pos = { 1,1,0,0 };
	myVertex[0].color = { 1,1,1,0 };

	myVertex[1].pos = { 1.f,0,0,0 };
	myVertex[1].color = { 1.f,0,1,0 };

	myVertex[2].pos = { -1,-1,0,0 };
	myVertex[2].color = { 0,1,1,0 };

	myVertex[3].pos = { 0,-1,0,0 };
	myVertex[3].color = { 0,1,0,0 };

	//Create vertex buffer
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };

	vertexData.pSysMem = myVertex;
	result = Engine::GetInstance()->GetGraphicsEngine().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
	if (FAILED(result))
	{
		return;
	}
}


void RayTracing::Update()
{}


void RayTracing::Render()
{
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->IASetInputLayout(myInputLayout.Get());
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->PSSetShader(myPixelShader.Get(), nullptr, 0);
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->VSSetShader(myVertexShader.Get(), nullptr, 0);


	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	Engine::GetInstance()->GetGraphicsEngine().GetContext()->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &stride, &offset);

	Engine::GetInstance()->GetGraphicsEngine().GetContext()->Draw(4, 0);

}