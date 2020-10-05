#include "main.h"
#include "renderer.h"
#include "texture.h"



void CTexture::Load(const char * FileName)
{
	unsigned char header[18];
	std::vector<char> image;
	unsigned int width, height;
	unsigned char depth;
	unsigned int bpp;
	unsigned int size;

	std::ifstream file(FileName, std::ios_base::in | std::ios_base::binary);

	assert(file);

	// ヘッダ読み込み
	file.read((char*)header, sizeof(header));

	// 画像サイズ取得
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	depth = header[16];

	if (depth == 32)
		bpp = 4;
	else if (depth == 24)
		bpp = 3;
	else
		bpp = 0;

	if (bpp != 4)
		assert(false);

	size = width * height * bpp;

	// メモリ確保
	image.resize(size);
	// 画像読み込み
	file.read(&image[0], size);





	file.close();




	ComPtr<ID3D12Device> device = CRenderer::GetInstance()->GetDevice();
	HRESULT hr{};

	// リソース作成
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	hr = device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_Resource));

	assert(SUCCEEDED(hr));

	//　デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.NumDescriptors = 1;
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDesc.NodeMask = 0;
	hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&m_descriptorHeap));
	assert(SUCCEEDED(hr));

	// シェーダーリソース作成
	D3D12_CPU_DESCRIPTOR_HANDLE handleSrv{};
	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};

	resourceViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	resourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = 1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.PlaneSlice = 0;
	resourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0F;
	resourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	handleSrv = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(*m_Resource.GetAddressOf(), &resourceViewDesc, handleSrv);

	// 画像データ書込
	D3D12_BOX box = { 0,0,0,(UINT)width, (UINT)height, 1 };
	hr = m_Resource->WriteToSubresource(0, &box, &image[0], 4 * width, 4 * width*height);
	assert(SUCCEEDED(hr));
}
