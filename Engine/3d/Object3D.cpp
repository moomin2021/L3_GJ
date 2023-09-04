#include "Object3D.h"
#include "DX12Cmd.h"
#include "Texture.h"
#include "Util.h"

// �ÓI�����o�ϐ��̎���
Camera*		Object3D::sCamera_		= nullptr;// �J����
LightGroup* Object3D::sLightGroup_	= nullptr;// ���C�g

Object3D::Object3D(Model* model) :
#pragma region ���������X�g
	// �I�u�W�F�N�g�f�[�^
	position_{ 0.0f, 0.0f, 0.0f },	// �ʒu(XYZ)
	rotation_{0.0f, 0.0f, 0.0f},	// ��](XYZ)
	scale_{1.0f, 1.0f, 1.0f},		// �g�k(XYZ)
	color_{1.0f, 1.0f, 1.0f, 1.0f},	// �F(RGBA)

	// �I�u�W�F�N�g�f�[�^��ύX�������ǂ���
	hasChanget_(true),

	// ���[���h�s��
	matWorld_{},

	// �萔�o�b�t�@
	constBuff_(nullptr),
	constMap_(nullptr),

	// ���f��
	model_(model)
#pragma endregion
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

#pragma region �萔�o�b�t�@����
	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region �萔�o�b�t�@�ւ̃f�[�^�]��
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	assert(SUCCEEDED(result));
#pragma endregion
}

void Object3D::Update()
{
	// �I�u�W�F�N�g�f�[�^���ύX����Ă����珈������
	if (hasChanget_) {
#pragma region ���[���h�s��v�Z
		// �s�񏉊���
		matWorld_ = Matrix4Identity();

		// ���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld_ *= Matrix4Scale(scale_);

		// ���[���h�s��ɉ�]�𔽉f
		matWorld_ *= Matrix4RotateZ(Util::Degree2Radian(rotation_.z));
		matWorld_ *= Matrix4RotateX(Util::Degree2Radian(rotation_.x));
		matWorld_ *= Matrix4RotateY(Util::Degree2Radian(rotation_.y));

		// ���[���h�s��ɕ��s�ړ��𔽉f
		matWorld_ *= Matrix4Translate(position_);
#pragma endregion

		// �ύX�����̂Ńt���O��[OFF]�ɂ���
		hasChanget_ = false;
	}

#pragma region �萔�o�b�t�@�ւ̃f�[�^�]��
	// �r���[�v���W�F�N�V�����]��
	constMap_->viewProj = sCamera_->GetMatView() * sCamera_->GetMatProjection();

	// ���[���h�s��]��
	constMap_->world = matWorld_;

	// �J�����̈ʒu���W(XYZ)�]��
	constMap_->cameraPos = sCamera_->GetEye();

	// �F(RGBA)�]��
	constMap_->color = color_;
#pragma endregion
}

void Object3D::Draw() {
	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());

	// ���C�g�̕`��
	sLightGroup_->Draw();

	// ���f���̏������ɕ`��
	model_->Draw();
}