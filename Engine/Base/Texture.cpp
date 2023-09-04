#include "Texture.h"
#include "DX12Cmd.h"
#include "float4.h"
#include "Util.h"
//#include "DirectXTex/d3dx12.h"

#include <vector>
#include <d3dx12.h>

using namespace DirectX;
using namespace std;

Texture* Texture::GetInstance() {
	// �C���X�^���X����
	static Texture instance;

	// �C���X�^���X��Ԃ�
	return &instance;
}

void Texture::Initialize() {
	// SRV�p�ŃX�N���v�^�q�[�v�𐶐�
	CreateDescriptorHeap();

	// �f�o�b�N�p�̃e�N�X�`����ǂݍ���
	LoadTexture("Resources/debug.png");
}

void Texture::LoadImageFile(const std::string filePath, DirectX::ScratchImage& scratchImage, DirectX::TexMetadata& metadata)
{
	HRESULT result;

	// WIC�e�N�X�`�����[�h
	result = LoadFromWICFile(
		Util::StringToWideChar(filePath).data(),
		WIC_FLAGS_FORCE_SRGB,
		&metadata, scratchImage
	);
	assert(SUCCEEDED(result));

	// �~�b�v�}�b�v����
	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImage.GetImages(),
		scratchImage.GetImageCount(),
		scratchImage.GetMetadata(),
		TEX_FILTER_SRGB,
		0, mipChain
	);

	// �~�b�v�}�b�v������ɐ������ꂽ��
	if (SUCCEEDED(result)) {
		scratchImage = move(mipChain);
		metadata = scratchImage.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);
}

ID3D12Resource* Texture::CreateTextureResource(const DirectX::TexMetadata& metadata, D3D12_RESOURCE_DESC& texResourceDesc)
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// ���\�[�X�ݒ�
	texResourceDesc.Width = UINT(metadata.width);
	texResourceDesc.Height = UINT(metadata.height);
	texResourceDesc.MipLevels = UINT16(metadata.mipLevels);
	texResourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	texResourceDesc.Format = metadata.format;
	texResourceDesc.SampleDesc.Count = 1;
	texResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	// ���p����Heap�̐ݒ�B
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// Resource�̐���
	ID3D12Resource* resource = nullptr;
	HRESULT result = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&texResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));

	return resource;
}

[[nodiscard]]
ID3D12Resource* Texture::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// Meta�����擾
	const TexMetadata& metadata = mipImages.GetMetadata();
	
	std::vector<D3D12_SUBRESOURCE_DATA> subResources;

	PrepareUpload(device, mipImages.GetImages(),
		mipImages.GetImageCount(), mipImages.GetMetadata(), subResources);

	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subResources.size()));
	ID3D12Resource* intermediateResource = CreateBufferResource(intermediateSize);
	UpdateSubresources(cmdList, texture, intermediateResource, 0, 0,
		UINT(subResources.size()), subResources.data());
	
	// Texture�ւ̓]����͗��p�ł���悤�ɁAD3D12_RESOURCE_STATE_COPY_DEST����
	// D3D12_RESOURCE_STATE_GENERIC_READ��ResourceState��ύX����
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	cmdList->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

ID3D12Resource* Texture::CreateBufferResource(size_t sizeInBytes)
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeInBytes;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// Resource�̐���
	ID3D12Resource* resource = nullptr;
	HRESULT result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));

	return resource;
}

uint16_t Texture::LoadTexture(const std::string fileName)
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// ���ɓǂݍ��񂾕���������
	if (texBuff_.find(fileName) != texBuff_.end()) {
		return texHandle_[fileName];
	}

	TexMetadata metadata{};
	ScratchImage scratchImage{};

	// �摜�ǂݍ���
	LoadImageFile(fileName, scratchImage, metadata);

	D3D12_RESOURCE_DESC texResourceDesc{};
	ID3D12Resource* texResource{};
	
	// �e�N�X�`���o�b�t�@�̐���
	texResource = CreateTextureResource(metadata, texResourceDesc);

	// �e�N�X�`���o�b�t�@�Ƀf�[�^��]��
	intermediateResources_.emplace_back(UploadTextureData(texResource, scratchImage));

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// CBV, SRV, UAV��1���̃T�C�Y���擾
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// �ǂݍ��񂾉摜�̃n���h��
	uint16_t handle = descriptorSize * loadCounter_;

	// �ݒ��ۑ�
	texBuff_.emplace(fileName, texResource);
	texHandle_.emplace(fileName, handle);

	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = texResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(texResourceDesc.MipLevels);

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff_[fileName].Get(), &srvDesc, srvHandle_);

	// �n���h����1�i�߂�
	srvHandle_.ptr += descriptorSize;

	// �J�E���^�[��i�߂�
	loadCounter_++;

	// �n���h����Ԃ�
	return handle;
}

void Texture::ReleaseIntermediateResources()
{
	if (intermediateResources_.empty()) return;

	for (auto& i : intermediateResources_) {
		i->Release();
	}

	intermediateResources_.clear();
}

//uint16_t Texture::LoadTexture(const std::string fileName)
//{
//	// �f�o�C�X�擾
//	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();
//
//	// ���ɓǂݍ��񂾕���������
//	if (texBuff_.find(fileName) != texBuff_.end()) {
//		return texHandle_[fileName];
//	}
//
//	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
//	HRESULT result;
//
//	TexMetadata metadata{};
//	ScratchImage scratchImg{};
//
//	// WIC�e�N�X�`���̃��[�h
//	result = LoadFromWICFile(
//		Util::StringToWideChar(fileName).data(),
//		WIC_FLAGS_NONE,
//		&metadata, scratchImg
//	);
//
//	ScratchImage mipChain{};
//
//	// �~�b�v�}�b�v����
//	result = GenerateMipMaps(
//		scratchImg.GetImages(),
//		scratchImg.GetImageCount(),
//		scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT,
//		0,
//		mipChain
//	);
//
//	if (SUCCEEDED(result)) {
//		scratchImg = std::move(mipChain);
//		metadata = scratchImg.GetMetadata();
//	}
//
//	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
//	metadata.format = MakeSRGB(metadata.format);
//
//	// �q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//
//	// ���\�[�X�ݒ�
//	D3D12_RESOURCE_DESC textureResourceDesc{};
//	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResourceDesc.Format = metadata.format;
//	textureResourceDesc.Width = metadata.width;
//	textureResourceDesc.Height = static_cast<uint32_t>(metadata.height);
//	textureResourceDesc.DepthOrArraySize = static_cast<uint32_t>(metadata.arraySize);
//	textureResourceDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);
//	textureResourceDesc.SampleDesc.Count = 1;
//
//	// �e�N�X�`���o�b�t�@�̐���
//	ComPtr<ID3D12Resource> texBuff = nullptr;
//	result = device->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff)
//	);
//
//	// �S�~�b�v�}�b�v�ɂ���
//	for (size_t i = 0; i < metadata.mipLevels; i++)
//	{
//		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
//		const Image* img = scratchImg.GetImage(i, 0, 0);
//
//		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
//		result = texBuff->WriteToSubresource(
//			static_cast<uint32_t>(i),
//			nullptr,								// �S�̈�փR�s�[
//			img->pixels,							// ���f�[�^�A�h���X
//			static_cast<uint32_t>(img->rowPitch),	// 1���C���T�C�Y
//			static_cast<uint32_t>(img->slicePitch)	// 1���T�C�Y
//		);
//		assert(SUCCEEDED(result));
//	}
//
//	// CBV, SRV, UAV��1���̃T�C�Y���擾
//	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//	// �ǂݍ��񂾉摜�̃n���h��
//	uint16_t handle = descriptorSize * loadCounter_;
//
//	// �ݒ��ۑ�
//	texBuff_.emplace(fileName, texBuff);
//	texHandle_.emplace(fileName, handle);
//
//	// �V�F�[�_���\�[�X�r���[�ݒ�
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//	srvDesc.Format = textureResourceDesc.Format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;
//
//	// �n���h���̎w���@�ɃV�F�[�_�[���\�[�X�r���[�쐬
//	device->CreateShaderResourceView(texBuff_[fileName].Get(), &srvDesc, srvHandle_);
//
//	// �n���h����1�i�߂�
//	srvHandle_.ptr += descriptorSize;
//
//	// �J�E���^�[��i�߂�
//	loadCounter_++;
//
//	// �n���h����Ԃ�
//	return handle;
//}


Texture::Texture() {}

void Texture::CreateDescriptorHeap()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = 2056;

	// �ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = DX12Cmd::GetInstance()->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap_));
	assert(SUCCEEDED(result));

	// SRV�q�[�v�̐擪�n���h�����擾
	srvHandle_ = srvHeap_->GetCPUDescriptorHandleForHeapStart();
}

uint16_t LoadTexture(const std::string fileName) {
	// �摜�ǂݍ���
	return Texture::GetInstance()->LoadTexture(fileName);
}
