#include "Sprite.h"
#include "DX12Cmd.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Util.h"

#include <DirectXMath.h>

using namespace DirectX;

Sprite::Sprite() :
#pragma region ���������X�g
	// �X�v���C�g�f�[�^
	position_{ 0.0f, 0.0f },			// ���W
	rotation_(0.0f),					// ��]
	size_{ 100.0f, 100.0f },			// �T�C�Y
	color_{ 1.0f, 1.0f, 1.0f, 1.0f },	// �F(RGBA)
	anchorPoint_{0.0f, 0.0f},			// �A���J�[�|�C���g(XY)
	isFlipX_(false),					// ���E���]
	isFlipY_(false),					// �㉺���]

	// �X�v���C�g�f�[�^��ύX�������ǂ���
	hasChanget_(true),

	// �s��
	matWorld_{},		// ���[���h�s��
	matProjection_{},	// �ˉe�ϊ��s��

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
	indexView_{}		// �C���f�b�N�X�o�b�t�@�r���[
#pragma endregion
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �C���X�^���X�擾
	WinAPI* winAPI = WinAPI::GetInstance();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// ���_�f�[�^
	vertex_[0] = { { 0.0f, 0.0f}, {0.0f, 1.0f} };// ����
	vertex_[1] = { { 0.0f, 0.0f}, {0.0f, 0.0f} };// ����
	vertex_[2] = { { 0.0f, 0.0f}, {1.0f, 1.0f} };// �E��
	vertex_[3] = { { 0.0f, 0.0f}, {1.0f, 0.0f} };// �E��

	// �C���f�b�N�X�f�[�^
	index_[0] = 0;
	index_[1] = 1;
	index_[2] = 2;
	index_[3] = 1;
	index_[4] = 2;
	index_[5] = 3;

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

#pragma region �C���f�b�N�X�o�b�t�@�m��
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y = �C���f�b�N�X�f�[�^����̃T�C�Y * �C���f�b�N�X�f�[�^�̗v�f��
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * index_.size());

	// �����[�X�ݒ�
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

	// �ˉe�ϊ��s�񏉊���
	XMMATRIX mat = XMMatrixOrthographicOffCenterLH(
		0.0f,
		static_cast<float>(winAPI->GetWidth()),
		static_cast<float>(winAPI->GetHeight()),
		0.0f, 0.0f, 1.0f);

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			matProjection_.m[i][j] = mat.r[i].m128_f32[j];
		}
	}
}

void Sprite::Draw(int textureHandle) {
	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �C���X�^���X�擾
	Texture* tex = Texture::GetInstance();

	// SRV�q�[�v�̃n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = tex->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// �X�v���C�g�f�[�^�̍X�V����
	UpdateData();

	// �n���h�����w�肳�ꂽ���܂Ői�߂�
	srvGpuHandle.ptr += textureHandle;

	// �w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	// �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vertexView_);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetIndexBuffer(&indexView_);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced(static_cast<UINT>(index_.size()), 1, 0, 0, 0);
}

void Sprite::UpdateData()
{
	// �X�v���C�g�f�[�^�̕ύX������Ă��Ȃ������珈�����΂�
	if (hasChanget_ == false) return;

#pragma region ���[���h�s��v�Z
	// �s�񏉊���
	matWorld_ = Matrix4Identity();

	// Z����]
	matWorld_ *= Matrix4RotateZ(Util::Degree2Radian(rotation_));

	// ���s�ړ�
	matWorld_ *= Matrix4Translate({position_.x, position_.y, 0.0f});
#pragma endregion

#pragma region �萔�o�b�t�@�̓]��
	// �s��v�Z
	constMap_->mat = matWorld_ * matProjection_;

	// �F(RGBA)
	constMap_->color = color_;
#pragma endregion

#pragma region ���_���W�ύX(�摜�̃T�C�Y��ύX)
	// ���_�f�[�^
	float left		= (0.0f - anchorPoint_.x);
	float right		= (1.0f - anchorPoint_.x);
	float top		= (0.0f - anchorPoint_.y);
	float bottom	= (1.0f - anchorPoint_.y);

	// ���E���]
	if (isFlipX_) left = -left, right = -right;

	// �㉺���]
	if (isFlipY_) top = -top, bottom = -bottom;

	vertex_[0] = { { left	* size_.x, bottom	* size_.y }, {0.0f, 1.0f} };// ����
	vertex_[1] = { { left	* size_.x, top		* size_.y }, {0.0f, 0.0f} };// ����
	vertex_[2] = { { right	* size_.x, bottom	* size_.y }, {1.0f, 1.0f} };// �E��
	vertex_[3] = { { right	* size_.x, top		* size_.y }, {1.0f, 0.0f} };// �E��

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