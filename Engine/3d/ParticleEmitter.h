#pragma once
#include "Camera.h"
#include "Vector3.h"
#include "Matrix4.h"

#include <d3d12.h>
#include <wrl.h>
#include <deque>

enum BILLBOARD {
	NONE,
	ALL,
	X,
	Y,
	Z
};

class ParticleEmitter
{
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �\����
	// ���_�f�[�^
	struct Vertex {
		Vector3 pos;
		float scale;
	};

	// �萔�o�b�t�@�p
	struct ConstBufferData {
		Matrix4 mat;
	};

	// �p�[�e�B�N��1��
	struct Particle {
		Vector3 position = {};	// ���W
		Vector3 velocity = {};	// ���x
		Vector3 accel = {};		// �����x
		uint16_t frame = 0;		// ���݃t���[��
		uint16_t num_frame = 0;	// �I���t���[��
		float scale = 1.0f;		// �X�P�[��
		float startScale = 1.0f;// �����l
		float endScale = 0.0f;	// �ŏI�l
	};
#pragma endregion

#pragma region �����o�ϐ�
	// �p�[�e�B�N���R���e�i
	std::deque<Particle> particles_;

	// �ő咸�_��
	const uint16_t MAX_VERTEX = 1024;

	// �����ʒu
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_ = nullptr;
	ConstBufferData* constMap_ = nullptr;

	// ���_�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vertexView_{};	// ���_�o�b�t�@�[�r���[
	ComPtr<ID3D12Resource> vertexBuff_ = nullptr;		// ���_�o�b�t�@

	// �C���f�b�N�X�f�[�^
	D3D12_INDEX_BUFFER_VIEW indexView_{};	// �C���f�b�N�X�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff_ = nullptr;	// �C���f�b�N�X�o�b�t�@

	// �J����
	static Camera* sCamera_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	ParticleEmitter();

	// �f�X�g���N�^
	~ParticleEmitter();

	// �X�V����
	void Update(BILLBOARD billBoard = BILLBOARD::NONE);

	// �`�揈��
	void Draw(uint16_t handle = 0);

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	/// <param name="life"> �������� </param>
	/// <param name="pos"> �������W </param>
	/// <param name="velocity"> ���x </param>
	/// <param name="accel"> �����x </param>
	void Add(uint16_t life, Vector3 pos, Vector3 velocity, Vector3 accel, float startScale, float endScale);

private:
	// �萔�o�b�t�@����
	void CreateConstBuff();

	// ���_�o�b�t�@����
	void CreateVertexBuff();

	// �C���f�b�N�X�o�b�t�@����
	void CreateIndexBuff();
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �����ʒu��ݒ�
	void SetSpawnPos(const Vector3& pos) { position_ = pos; }

	// �J������ݒ�
	static void SetCamera(Camera* camera) { sCamera_ = camera; }
#pragma endregion
};