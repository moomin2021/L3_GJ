#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "Matrix4.h"

#include <d3d12.h>
#include <wrl.h>
#include <deque>

class ParticleEmitter2D
{
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma region �\����
private:
	// ���_�f�[�^
	struct Vertex {
		Vector2 pos = { 0.0f, 0.0f };
		float pad;
		float scale = 0.0f;
	};

	// �萔�o�b�t�@�p
	struct ConstBufferData {
		Matrix4 mat;
	};

	// �p�[�e�B�N��
	struct Particle {
		Vector2 position = { 0.0f, 0.0f };
		Vector2 velocity = { 0.0f, 0.0f };
		Vector2 accel = { 0.0f, 0.0f };
		uint16_t frame = 0;
		uint16_t num_frame = 0;
		float scale = 1.0f;
		float startScale = 1.0f;
		float endScale = 0.0f;
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// �p�[�e�B�N���R���e�i
	std::deque<Particle> particles_;

	// �ő咸�_��
	const uint16_t MAX_VERTEX = 1024;

	// �����ʒu
	Vector2 position_ = { 0.0f, 0.0f };

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
	ParticleEmitter2D(uint16_t maxVertex);

	// �f�X�g���N�^
	~ParticleEmitter2D();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw(uint16_t handle = 0);

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	/// <param name="life"> �������� </param>
	/// <param name="pos"> �������W </param>
	/// <param name="velocity"> ���x </param>
	/// <param name="accel"> �����x </param>
	void Add(uint16_t life, const Vector2& pos, const Vector2& velocity, const Vector2& accel, float startScale, float endScale);

private:
	// �萔�o�b�t�@����
	void CreateConstBuff();

	// ���_�o�b�t�@����
	void CreateVertexBuff(uint16_t maxVertex);

	// �C���f�b�N�X�o�b�t�@����
	void CreateIndexBuff();
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// ���W��ݒ�
	void SetPosition(const Vector2& pos) { position_ = pos; }

	// �J������ݒ�
	static void SetCamera(Camera* camera) { sCamera_ = camera; }
#pragma endregion
};