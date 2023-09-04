#include "Material.h"
#include "DX12Cmd.h"
#include "Texture.h"

#include <cassert>

Material::Material() :
#pragma region ���������X�g
	// �}�e���A���f�[�^
	name_{},				// �}�e���A����
	ambient_{},				// �A���r�G���g�e���x
	diffuse_{},				// �f�B�t���[�Y�e���x
	specular_{},			// �X�y�L�����[�e���x
	alpha_(1.0f),			// �A���t�@�l
	texHandle_(0),			// �e�N�X�`���n���h��
	materialBuff_(nullptr)	// �}�e���A���o�b�t�@
#pragma endregion
{

}

void Material::Draw()
{
	// �R�}���h���X�g�擾
	static ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �C���X�^���X�擾
	static Texture* tex = Texture::GetInstance();

	// SRV�q�[�v�̃n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = tex->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// �n���h�����w�肳�ꂽ���܂Ői�߂�
	srvGpuHandle.ptr += texHandle_;

	// �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(2, materialBuff_->GetGPUVirtualAddress());

	// �w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);
}

void Material::CreateMaterialBuff()
{
	// �f�o�C�X�擾
	static ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(MaterialBuffer) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&materialBuff_));
	assert(SUCCEEDED(result));

	// �}�e���A���萔�o�b�t�@�̃}�b�s���O
	MaterialBuffer* materialMap;
	result = materialBuff_->Map(0, nullptr, (void**)&materialMap);
	assert(SUCCEEDED(result));
	materialMap->ambient = ambient_;
	materialMap->diffuse = diffuse_;
	materialMap->specular = specular_;
	materialMap->alpha = alpha_;
	materialBuff_->Unmap(0, nullptr);
}