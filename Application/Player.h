#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"
#include"BoxCollider.h"
#include"CollisionManager2D.h"

class Player {
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Pad* pad_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// �摜�n���h��
	uint16_t handle_ = 0;

	// �R���C�_�[
	std::unique_ptr<BoxCollider> collider_ = nullptr;

	// ���W
	Vector2 position_ = { 96.0f, 540.0f };

	// ��]�p
	float rotate_ = 0.0f;
	float beforeRotate_ = 0.0f;	// ��]�O��]�p
	float afterRotate_ = 0.0f;	// ��]���]�p

	// �ړ����x
	float baseSpd_ = 8.0f;

	// ��]����[s]
	float rotateTime_ = 0.5f;

	// ��]�J�n����
	uint64_t startRotateTime_ = 0;

	// �u���b�N
	std::vector<Block*> blocks_{};

#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ��X�V����
	void OnCollision();

	// �s��X�V
	void MatUpdate();

private:
	// �ړ�����
	void Move();

	// ��]����
	void Rotate();
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	// �v���C���[�̍��p���擾
	const Vector2& GetPosition() { return sprite_->GetPosition(); }
#pragma endregion
};