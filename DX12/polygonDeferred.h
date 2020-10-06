#pragma once
#include "main.h"
#include "texture.h"

using namespace Microsoft::WRL;

class CPolygonDeferred
{
private:
	ComPtr<ID3D12Resource>	m_VertexBuffer;
	ComPtr<ID3D12Resource>	m_ConstantBuffer;

public:
	void Initialize();
	void Uninitialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList *CommandList, ID3D12DescriptorHeap* Texture);
};


