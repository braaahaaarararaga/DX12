#include "main.h"
#include "renderer.h"
#include "polygon.h"

void CPolygon::Initialize()
{
	ComPtr<ID3D12Device> device = CRenderer::GetInstance()->GetDevice();

	HRESULT hr{};
	D3D12_HEAP_PROPERTIES heapProperties{};
	D3D12_RESOURCE_DESC	  resourceDesc{};

	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;

	// 픵�Хåե�������
	resourceDesc.Width = sizeof(Vertex3D) * 4;
	hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_VertexBuffer));
	assert(SUCCEEDED(hr));

	// �����Хåե�������
	resourceDesc.Width = 256; // �����Хåե���256byte���饤��
	hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_ConstantBuffer));
	assert(SUCCEEDED(hr));

	// 픵�ǩ`���Ε����z��
	Vertex3D *buffer{};
	hr = m_VertexBuffer->Map(0, nullptr, (void**)&buffer);
	assert(SUCCEEDED(hr));

	buffer[0].Position = { 0.0f, 0.0f, 0.0f };
	buffer[1].Position = { 100.0f, 0.0f, 0.0f };
	buffer[2].Position = { 0.0f, 100.0f, 0.0f };
	buffer[3].Position = { 100.0f, 100.0f, 0.0f };
	buffer[0].Normal = { 0.0f, 1.0f, 0.0f };
	buffer[1].Normal = { 0.0f, 1.0f, 0.0f };
	buffer[2].Normal = { 0.0f, 1.0f, 0.0f };
	buffer[3].Normal = { 0.0f, 1.0f, 0.0f };
	buffer[0].TexCoord = { 0.0f, 0.0f };
	buffer[1].TexCoord = { 1.0f, 0.0f };
	buffer[2].TexCoord = { 0.0f, 1.0f };
	buffer[3].TexCoord = { 1.0f, 1.0f };

	m_VertexBuffer->Unmap(0, nullptr);


	// �ƥ��������i���z��
	m_Texture.Load("data/field004.tga");
}

void CPolygon::Uninitialize()
{
}

void CPolygon::Update()
{
}

void CPolygon::Draw(ID3D12GraphicsCommandList * CommandList)
{
	HRESULT hr;

	// �ޥȥꥯ���O��
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	XMMATRIX world = XMMatrixTranslation(100.0f, 100.0f, 0.0f);

	// �����Хåե��O��
	Constant *constant;
	hr = m_ConstantBuffer->Map(0, nullptr, (void**)&constant);
	assert(SUCCEEDED(hr));

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * view * projection));
	constant->WVP = matrix;
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(world));
	constant->World = matrix;

	m_ConstantBuffer->Unmap(0, nullptr);

	CommandList->SetGraphicsRootConstantBufferView(0, m_ConstantBuffer->GetGPUVirtualAddress());

	// 픵�Хåե��O��
	D3D12_VERTEX_BUFFER_VIEW vertexView{};
	vertexView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	vertexView.StrideInBytes = sizeof(Vertex3D);
	vertexView.SizeInBytes = sizeof(Vertex3D) * 4;
	CommandList->IASetVertexBuffers(0, 1, &vertexView);

	// �ƥ��������O��
	ID3D12DescriptorHeap* dh[] = { *m_Texture.GetSDescriptorHeap().GetAddressOf() };
	CommandList->SetDescriptorHeaps(_countof(dh), dh);
	CommandList->SetGraphicsRootDescriptorTable(1,
		m_Texture.GetSDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

	// �ȥݥ��O��
	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �軭
	CommandList->DrawInstanced(4, 1, 0, 0);
}
