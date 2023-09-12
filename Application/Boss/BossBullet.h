#pragma once
#include "Sprite.h"
#include "BoxCollider.h"
#include "CollisionManager2D.h"

#include <memory>

class BossBullet
{
#pragma region �����o�ϐ�
private:
	// �摜�n���h��
	static uint16_t sHandle_;

	// �e�̑��x
	static float sSpeed_;

	// �Փ˃}�l�[�W���[2D
	static CollisionManager2D* sColMgr2D_;

	// �X�v���C�g
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// �R���C�_�[
	std::unique_ptr<BoxCollider> collider_ = nullptr;

	// ���W
	Vector2 position_ = { 0.0f, 0.0f };

	// �����t���O
	bool isAlive_ = true;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	BossBullet();

	// �f�X�g���N�^
	~BossBullet();

	// ����������
	void Initialize(Vector2 pos);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�����
	void OnCollision();

	// �s��X�V
	void MatUpdate();
#pragma endregion

#pragma region �Z�b�^�[�֐�
	// �n���h����ݒ�
	static void SetHandle(uint16_t handle) { sHandle_ = handle; }

	// ���x��ݒ�
	static void SetSpeed(float speed) { sSpeed_ = speed; }

	// �Փ˃}�l�[�W���[��ݒ�
	static void SetColMgr2D(CollisionManager2D* colMgr2D) { sColMgr2D_ = colMgr2D; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �����t���O���擾
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};