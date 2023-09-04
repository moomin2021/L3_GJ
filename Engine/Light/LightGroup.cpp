#include "LightGroup.h"
#include "DX12Cmd.h"

#include <d3dx12.h>
#include <cassert>

LightGroup::LightGroup() :
#pragma region ���������X�g
	// �萔�o�b�t�@
	constBuff_(nullptr),

	// �����̐F
	ambientColor_{ 1.0f, 1.0f, 1.0f },
	
	// ���C�g�f�[�^
	dirLights_(DIR_LIGHT_LIMIT),		// ���s�����̔z��
	pointLights_(POINT_LIGHT_LIMIT),	// �_�����̔z��
	spotLights_(SPOT_LIGHT_LIMIT),		// �X�|�b�g���C�g�p
	circleShadows_(CIRCLE_SHADOW_LIMIT),// �ۉe�̔z��

	// �_�[�e�B�t���O
	dirty_(false)
#pragma endregion
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �q�[�v�ݒ�
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void LightGroup::Draw() {
	// �萔�o�b�t�@�]��
	TransferConstBuffer();

	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(3, constBuff_->GetGPUVirtualAddress());
}

void LightGroup::AddDirLight(DirectionalLight* light) {
	dirLights_[0] = light;
}

void LightGroup::AddPointLight(PointLight* light) {
	for (size_t i = 0; i < POINT_LIGHT_LIMIT; i++) {
		// ���g���󂾂�����
		if (pointLights_[i] == nullptr) {
			pointLights_[i] = light;
			break;
		}
	}
}

void LightGroup::TransferConstBuffer() {

	// �_�[�e�B�t���O��[OFF]�Ȃ珈�����΂�
	//if (dirty_ == false) return;

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

#pragma region �萔�o�b�t�@�}�b�s���O
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
#pragma endregion

	if (SUCCEEDED(result)) {
#pragma region ���s����
		// ���s����
		for (size_t i = 0; i < DIR_LIGHT_LIMIT; i++) {
			// ���g����ł͂Ȃ�������
			if (dirLights_[i] != nullptr) {
				// ���C�g���L���Ȃ�ݒ��]��
				if (dirLights_[i]->GetActive()) {
					constMap->dirLights[i].active = true;
					constMap->dirLights[i].lightVec = -dirLights_[i]->GetLightDir();
					constMap->dirLights[i].lightColor = dirLights_[i]->GetLightColor();
				}

				// ���C�g�������Ȃ�]�����Ȃ�
				else {
					constMap->dirLights[i].active = false;
				}
			}
		}
#pragma endregion

#pragma region �_����
		for (size_t i = 0; i < POINT_LIGHT_LIMIT; i++) {
			// ���g����ł͂Ȃ�������
			if (pointLights_[i] != nullptr) {
				// ���C�g���L���Ȃ�ݒ��]��
				if (pointLights_[i]->GetActive()) {
					constMap->pointLights[i].active = true;
					constMap->pointLights[i].lightPos = pointLights_[i]->GetLightPos();
					constMap->pointLights[i].lightColor = pointLights_[i]->GetLightColor();
					constMap->pointLights[i].lightAtten = pointLights_[i]->GetLightAtten();
				}

				// ���C�g�������Ȃ烉�C�g�F��0��
				else {
					constMap->pointLights[i].active = false;
				}
			}
		}
#pragma endregion

#pragma region �X�|�b�g���C�g
		for (size_t i = 0; i < SPOT_LIGHT_LIMIT; i++) {
			// ���g����ł͂Ȃ�������
			if (spotLights_[i] != nullptr) {
				// ���C�g���L���Ȃ�ݒ��]��
				if (spotLights_[i]->GetActive()) {
					constMap->spotLights[i].active = true;
					constMap->spotLights[i].lightVec = -spotLights_[i]->GetLightDir();
					constMap->spotLights[i].lightPos = spotLights_[i]->GetLightPos();
					constMap->spotLights[i].lightColor = spotLights_[i]->GetLightColor();
					constMap->spotLights[i].lightAtten = spotLights_[i]->GetLightAtten();
					constMap->spotLights[i].lightFactorAngleCos = spotLights_[i]->GetLightFactorAngle();
				}

				// ���C�g�������Ȃ烉�C�g�F��0��
				else {
					constMap->spotLights[i].active = false;
				}
			}
		}
#pragma endregion

#pragma region �ۉe
		// �ۉe
		for (size_t i = 0; i < CIRCLE_SHADOW_LIMIT; i++) {
			// ���g��ł͂Ȃ�������
			if (circleShadows_[i] != nullptr) {
				// �L���Ȃ�ݒ��]��
				if (circleShadows_[i]->GetActive()) {
					constMap->circleShadows[i].active = true;
					constMap->circleShadows[i].dir = -circleShadows_[i]->GetDir();
					constMap->circleShadows[i].casterPos = circleShadows_[i]->GetCasterPos();
					constMap->circleShadows[i].distCasterLight = circleShadows_[i]->GetDistCasterLight();
					constMap->circleShadows[i].atten = circleShadows_[i]->GetAtten();
					constMap->circleShadows[i].factorAngleCos = circleShadows_[i]->GetFactorAngleCos();
				}

				// �����Ȃ�F��0��
				else {
					constMap->circleShadows[i].active = false;
				}
			}
		}
#pragma endregion

		// ����
		constMap->ambientColor = ambientColor_;

		// �}�b�s���O�㏈��
		constBuff_->Unmap(0, nullptr);
	}

	// �]�����I������̂Ń_�[�e�B�t���O��[OFF]�ɂ���
	dirty_ = false;
}