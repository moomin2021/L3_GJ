#include "ParticleEmitter2D.h"
#include "DX12Cmd.h"
#include "Texture.h"

#include <cassert>

Camera* ParticleEmitter2D::sCamera_ = nullptr;

ParticleEmitter2D::ParticleEmitter2D(uint16_t maxVertex)
{
	CreateConstBuff();// �萔�o�b�t�@����
	CreateVertexBuff(maxVertex);// ���_�o�b�t�@����
	CreateIndexBuff();// �C���f�b�N�X�o�b�t�@����
}

ParticleEmitter2D::~ParticleEmitter2D() {}

void ParticleEmitter2D::Update()
{
	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

	// �������s�����p�[�e�B�N����S�폜
	for (auto it = particles_.begin(); it != particles_.end();) {
		// ������v�����v�f���폜����
		if ((*it).frame >= (*it).num_frame) {
			// �폜���ꂽ�v�f�̎����w���C�e���[�^���Ԃ����B
			it = particles_.erase(it);
		}

		// �v�f�폜�����Ȃ��ꍇ�ɁA�C�e���[�^��i�߂�
		else {
			++it;
		}
	}

	// �S�p�[�e�B�N���X�V
	for (auto& it : particles_) {
		// �o�߃t���[�������J�E���g
		it.frame++;

		// ���x�ɉ����x�����Z
		it.velocity = (it.velocity + it.accel) * it.speed;

		// ���x�ɂ��ړ�
		it.position = it.position + it.velocity;

		// �i�s�x��0~1�͈̔͂Ɋ��Z
		float f = (float)it.frame / it.num_frame;

		// �X�P�[���̐��`���
		it.scale = (it.endScale - it.startScale) * f;
		it.scale += it.startScale;
	}

	// ���_�o�b�t�@�փf�[�^�]��
	Vertex* vertMap = nullptr;
	result = vertexBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		// �p�[�e�B�N���̏���1�����f
		for (auto& it : particles_) {
			// ���W
			vertMap->pos = it.position;
			vertMap->scale = it.scale;

			vertMap++;
		}

		vertexBuff_->Unmap(0, nullptr);
	}

#pragma region ���[���h�s��v�Z
	// �s�񏉊���
	Matrix4 matWorld = Matrix4Identity();

	// ���s�ړ�
	matWorld *= Matrix4Translate({ position_.x, position_.y, 0.0f });
#pragma endregion

#pragma region �萔�o�b�t�@�ւ̃f�[�^�]��
	// �s��
	constMap_->mat = matWorld * sCamera_->GetMatOrthoGraphicPro();
#pragma endregion
}

void ParticleEmitter2D::Draw(uint16_t handle)
{
	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* cmdList = DX12Cmd::GetInstance()->GetCmdList();

	// �C���X�^���X�擾
	Texture* tex = Texture::GetInstance();

	// SRV�q�[�v�̃n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = tex->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// �n���h�����w�肳�ꂽ���܂Ői�߂�
	srvGpuHandle.ptr += handle;

	// �w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);

	// �萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff_->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vertexView_);

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetIndexBuffer(&indexView_);

	// �`��R�}���h
	cmdList->DrawInstanced(static_cast<UINT>(std::distance(particles_.begin(), particles_.end())), 1, 0, 0);
}

void ParticleEmitter2D::Add(uint16_t life, const Vector2& pos, const Vector2& velocity, const Vector2& accel, float startScale, float endScale)
{
	// ���X�g�ɗv�f��ǉ�
	particles_.emplace_front();
	// �ǉ������v�f�̎Q��
	Particle& p = particles_.front();
	// �l�̃Z�b�g
	p.position = pos;
	p.velocity.x = velocity.x;
	p.velocity.y = -velocity.y;
	p.accel.x = accel.x;
	p.accel.y = -accel.y;
	p.startScale = startScale;
	p.endScale = endScale;
	p.num_frame = life;
}

void ParticleEmitter2D::Add(uint16_t life, const Vector2& pos, const Vector2& velocity, float speed, float startScale, float endScale)
{
	// ���X�g�ɗv�f��ǉ�
	particles_.emplace_front();
	// �ǉ������v�f�̎Q��
	Particle& p = particles_.front();
	// �l�̃Z�b�g
	p.position = pos;
	p.velocity = velocity;
	p.speed = speed;
	p.startScale = startScale;
	p.endScale = endScale;
	p.num_frame = life;
}

void ParticleEmitter2D::CreateConstBuff()
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

void ParticleEmitter2D::CreateVertexBuff(uint16_t maxVertex)
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * maxVertex);

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

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
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuff_));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�r���[�̍쐬
	vertexView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();// GPU���z�A�h���X
	vertexView_.SizeInBytes = sizeVB;				// ���_�o�b�t�@�̃T�C�Y
	vertexView_.StrideInBytes = sizeof(Vertex);	// ���_1���̃f�[�^�T�C�Y
}

void ParticleEmitter2D::CreateIndexBuff()
{
	// �f�o�C�X�擾
	ID3D12Device* device = DX12Cmd::GetInstance()->GetDevice();

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * 6);

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp,// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff_)
	);

	// �C���f�b�N�X�o�b�t�@�r���[�쐬
	indexView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	indexView_.Format = DXGI_FORMAT_R16_UINT;
	indexView_.SizeInBytes = sizeIB;

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// �C���f�b�N�X���𑗐M
	indexMap[0] = 0;
	indexMap[1] = 1;
	indexMap[2] = 2;
	indexMap[3] = 2;
	indexMap[4] = 1;
	indexMap[5] = 3;

	// �}�b�s���O����
	indexBuff_->Unmap(0, nullptr);
}