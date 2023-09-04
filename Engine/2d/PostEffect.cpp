#include "PostEffect.h"
#include "DX12Cmd.h"
#include "Texture.h"
#include "WinAPI.h"
#include "Util.h"

#include <DirectXMath.h>
#include <d3dx12.h>

using namespace DirectX;

const std::vector<float> PostEffect::clearColor_ = { 0.1f, 0.25, 0.5f, 0.0f };

PostEffect::PostEffect() :
#pragma region ���������X�g
	// �X�v���C�g�f�[�^
	scale_{1.0f, 1.0f},// �g�k(��ʂ̑傫���ɑ΂���)

	// �X�v���C�g�f�[�^��ύX�������ǂ���
	hasChanget_(true),

	// �萔�o�b�t�@
	constBuff_(nullptr),// �萔�o�b�t�@
	constMap_(nullptr),	// �}�b�s���O�����p

	// ���_�f�[�^
	vertex_(4),
	vertexBuff_(nullptr),	// ���_�o�b�t�@
	vertexMap_(nullptr),	// �}�b�s���O�����p
	vertexView_{},			// ���_�o�b�t�@�r���[

	// �C���f�b�N�X�f�[�^
	index_(6),
	indexBuff_(nullptr),// �C���f�b�N�X�o�b�t�@
	indexView_{},		// �C���f�b�N�X�o�b�t�@�r���[

	// �e�N�X�`���o�b�t�@
	texBuff_(2),
	
	// SRV�p�f�X�N���v�^�q�[�v
	descHeapSRV_(nullptr),
	
	// �[�x�o�b�t�@
	depthBuff_(nullptr),
	
	// RTV�p�f�X�N���v�^�q�[�v
	descHeapRTV_(nullptr),
	
	// DSV�p�f�X�N���v�^�q�[�v
	descHeapDSV_(nullptr)
#pragma endregion
{
	// ���_�o�b�t�@�̍쐬
	CreateVertexBuff();

	// �C���f�b�N�X�o�b�t�@�̍쐬
	CreateIndexBuff();

	// �萔�o�b�t�@�̍쐬
	CreateConstBuff();

	// �e�N�X�`���o�b�t�@�쐬
	CreateTextureBuff();

	// SRV�p�f�X�N���v�^�q�[�v�쐬
	CreateSRVDescHeap();

	// RTV�p�f�X�N���v�^�q�[�v�쐬
	CreateRTVDescHeap();

	// �[�x�o�b�t�@�쐬
	CreateDepthBuff();

	// DSV�p�f�X�N���v�^�q�[�v�쐬
	CreateDSVDescHeap();
}

void PostEffect::Draw(bool isDescHeap)
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �X�v���C�g�f�[�^�̍X�V����
	UpdateData();

	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	if (isDescHeap) {
		std::vector<ID3D12DescriptorHeap*> ppHeaps = { descHeapSRV_.Get() };
		cmdList->SetDescriptorHeaps(1, ppHeaps.data());
	}

	// �w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�
	CD3DX12_GPU_DESCRIPTOR_HANDLE descHandle0 = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descHeapSRV_->GetGPUDescriptorHandleForHeapStart(), 0,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	cmdList->SetGraphicsRootDescriptorTable(0, descHandle0);

	// �w�肳�ꂽSRV�����[�g�p�����[�^2�Ԃɐݒ�
	CD3DX12_GPU_DESCRIPTOR_HANDLE descHandle1 = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descHeapSRV_->GetGPUDescriptorHandleForHeapStart(), 1,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	cmdList->SetGraphicsRootDescriptorTable(1, descHandle1);

	// �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(2, constBuff_->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vertexView_);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetIndexBuffer(&indexView_);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced(static_cast<UINT>(index_.size()), 1, 0, 0, 0);
}

void PostEffect::PreDraw()
{
	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �E�B���h�E�T�C�Y���擾
	float winWidth = static_cast<float>(WinAPI::GetInstance()->GetWidth());
	float winHeight = static_cast<float>(WinAPI::GetInstance()->GetHeight());

	for (size_t i = 0; i < 2; i++) {
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff_[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);

		// ���\�[�X�o���A��ύX(�V�F�[�_�[���\�[�X->�`��\)
		cmdList->ResourceBarrier(1, &resourceBarrier);
	}

	// �����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHs(2);

	for (size_t i = 0; i < 2; i++) {
		CD3DX12_CPU_DESCRIPTOR_HANDLE descHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(i),
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

		rtvHs[i] = descHandle;
	}

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();

	// �����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(2, rtvHs.data(), false, &dsvH);

	// �r���[�|�[�g�ݒ�
	std::vector<CD3DX12_VIEWPORT> viewPorts(2);

	for (size_t i = 0; i < 2; i++) {
		viewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, winWidth, winHeight);
	}

	cmdList->RSSetViewports(1, viewPorts.data());

	// �V�U�����O��`�̐ݒ�
	std::vector<CD3DX12_RECT> rects(2);

	for (size_t i = 0; i < 2; i++) {
		rects[i] = CD3DX12_RECT(0, 0, static_cast<LONG>(winWidth), static_cast<LONG>(winHeight));
	}

	cmdList->RSSetScissorRects(
		2, rects.data());

	// �S��ʃN���A
	for (size_t i = 0; i < 2; i++) {
		cmdList->ClearRenderTargetView(rtvHs[i], clearColor_.data(), 0, nullptr);
	}

	// �[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(
		dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDraw()
{
	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// ���\�[�X�o���A��ύX(�`��\->�V�F�[�_�[���\�[�X)
	for (size_t i = 0; i < 2; i++) {
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(texBuff_[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		cmdList->ResourceBarrier(1, &resourceBarrier);
	}
}

void PostEffect::UpdateData()
{
	// �X�v���C�g�f�[�^�̕ύX������Ă��Ȃ������珈�����΂�
	if (hasChanget_ == false) return;

#pragma region ���_���W�ύX(�摜�̃T�C�Y��ύX)

	// ���_�f�[�^
	vertex_[0] = { { -scale_.x, -scale_.y }, {0.0f, 1.0f} };// ����
	vertex_[1] = { { -scale_.x, +scale_.y }, {0.0f, 0.0f} };// ����
	vertex_[2] = { { +scale_.x, -scale_.y }, {1.0f, 1.0f} };// �E��
	vertex_[3] = { { +scale_.x, +scale_.y }, {1.0f, 0.0f} };// �E��

	// �S���_�ɑ΂���
	for (size_t i = 0; i < vertex_.size(); i++)
	{
		// ���W���R�s�[
		vertexMap_[i] = vertex_[i];
	}
#pragma endregion

	// �ύX�����̂Ńt���O��[OFF]�ɂ���
	hasChanget_ = false;
}

void PostEffect::CreateVertexBuff()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// ���_�f�[�^
	vertex_[0] = { { -1.0f, -1.0f }, {0.0f, 1.0f} };// ����
	vertex_[1] = { { -1.0f, +1.0f }, {0.0f, 0.0f} };// ����
	vertex_[2] = { { +1.0f, -1.0f }, {1.0f, 1.0f} };// �E��
	vertex_[3] = { { +1.0f, +1.0f }, {1.0f, 0.0f} };// �E��

#pragma region ���_�o�b�t�@�m��
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertex_[0]) * vertex_.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapVSProp{};			// �q�[�v�ݒ�
	heapVSProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapVSProp,// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff_));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region ���_�o�b�t�@�ւ̃f�[�^�`��
	// Map�����Ń��C����������GPU�̃�������R�Â���
	result = vertexBuff_->Map(0, nullptr, (void**)&vertexMap_);
	assert(SUCCEEDED(result));

	// �S���_�ɑ΂���
	for (size_t i = 0; i < vertex_.size(); i++)
	{
		// ���W���R�s�[
		vertexMap_[i] = vertex_[i];
	}
#pragma endregion

#pragma region ���_�o�b�t�@�r���[�쐬

	// GPU���z�A�h���X
	vertexView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();

	// ���_�o�b�t�@�̃T�C�Y
	vertexView_.SizeInBytes = sizeVB;

	// ���_1���̃f�[�^�T�C�Y
	vertexView_.StrideInBytes = sizeof(vertex_[0]);
#pragma endregion
}

void PostEffect::CreateIndexBuff()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �C���f�b�N�X�f�[�^
	index_[0] = 0;
	index_[1] = 1;
	index_[2] = 2;
	index_[3] = 1;
	index_[4] = 2;
	index_[5] = 3;

#pragma region �C���f�b�N�X�o�b�t�@�m��
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y = �C���f�b�N�X�f�[�^����̃T�C�Y * �C���f�b�N�X�f�[�^�̗v�f��
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * index_.size());

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapVSProp{};			// �q�[�v�ݒ�
	heapVSProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// �����[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapVSProp,// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	// �}�b�s���O�p
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);

	// �S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < index_.size(); i++) {
		indexMap[i] = index_[i];
	}

	// �}�b�s���O����
	indexBuff_->Unmap(0, nullptr);
#pragma endregion

#pragma region �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	indexView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();

	// �t�H�[�}�b�g�w��
	indexView_.Format = DXGI_FORMAT_R16_UINT;

	// �C���f�b�N�X�f�[�^1���̃T�C�Y
	indexView_.SizeInBytes = sizeIB;
#pragma endregion
}

void PostEffect::CreateConstBuff()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

#pragma region �萔�o�b�t�@����
	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region �萔�o�b�t�@�ւ̃f�[�^�]��
	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
#pragma endregion
}

void PostEffect::CreateTextureBuff()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �E�B���h�E�T�C�Y���擾
	float winWidth = static_cast<float>(WinAPI::GetInstance()->GetWidth());
	float winHeight = static_cast<float>(WinAPI::GetInstance()->GetHeight());

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC textureDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			static_cast<UINT>(winWidth), static_cast<UINT>(winHeight),
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_HEAP_PROPERTIES texHeapProp = CD3DX12_HEAP_PROPERTIES(
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE texClearValue =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor_.data());

	// �e�N�X�`���o�b�t�@�̐���
	for (size_t i = 0; i < 2; i++) {
		result = device->CreateCommittedResource(
			&texHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&texClearValue,
			IID_PPV_ARGS(&texBuff_[i]));
		assert(SUCCEEDED(result));

		// �C���[�W�f�[�^
		{
			// ��f��
			const UINT pixelCount = static_cast<UINT>(winWidth * winHeight);

			// �摜��s���̃f�[�^�T�C�Y
			const UINT rowPitch = sizeof(UINT) * static_cast<UINT>(winWidth);

			const UINT depthPitch = rowPitch * static_cast<UINT>(winHeight);

			// �摜�C���[�W
			std::vector<UINT> img(pixelCount);
			for (size_t j = 0; j < pixelCount; j++) { img[j] = 0xff0000ff; }

			// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
			result = texBuff_[i]->WriteToSubresource(
				0, nullptr, img.data(), rowPitch, depthPitch);
			assert(SUCCEEDED(result));
		}
	}
}

void PostEffect::CreateSRVDescHeap()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;

	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(
		&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV_));
	assert(SUCCEEDED(result));

	// SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};// �ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;// 2D�e�N�X�`��
	srvDesc.Texture3D.MipLevels = 1;

	// �f�X�N���v�^�q�[�v��SRV�쐬
	for (size_t i = 0; i < 2; i++) {
		CD3DX12_CPU_DESCRIPTOR_HANDLE descHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapSRV_->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(i),
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

		device->CreateShaderResourceView(texBuff_[i].Get(),// �r���[�Ɗ֘A�t����o�b�t�@
			&srvDesc, descHandle);
	}
}

void PostEffect::CreateRTVDescHeap()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	// RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(
		&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
	assert(SUCCEEDED(result));

	// �f�X�N���v�^�q�[�v��RTV�쐬
	for (size_t i = 0; i < 2; i++) {
		device->CreateRenderTargetView(
			texBuff_[i].Get(), nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(i),
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}
}

void PostEffect::CreateDepthBuff()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �E�B���h�E�T�C�Y���擾
	float winWidth = static_cast<float>(WinAPI::GetInstance()->GetWidth());
	float winHeight = static_cast<float>(WinAPI::GetInstance()->GetHeight());

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			static_cast<UINT64>(winWidth),
			static_cast<UINT>(winHeight),
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_HEAP_PROPERTIES depthHeapPro =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE depthClearValue =
		CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);


	// �[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&depthHeapPro,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_));
	assert(SUCCEEDED(result));
}

void PostEffect::CreateDSVDescHeap()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 1;

	// DRV�p�f�X�N���v�^�q�[�v���쐬
	result = device->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	// �f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;// �[�x�n�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff_.Get(),
		&dsvDesc, descHeapDSV_->GetCPUDescriptorHandleForHeapStart());
}