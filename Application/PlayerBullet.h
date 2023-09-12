#pragma once
#include "Sprite.h"
#include "Boss.h"
#include "CollisionManager2D.h"
#include "BoxCollider.h"

#include<memory>

class PlayerBullet {
#pragma region �����o�ϐ�
private:
	static Vector2	sBulletSize_;	// �e�̃T�C�Y
	static uint16_t	sHandle_;		// �e�̉摜�n���h��
	static Boss*	sBoss_;			// �{�X�̃|�C���^
	static CollisionManager2D* sColMgr2D_;// �Փ˃}�l�[�W���[2D

	// ���W
	Vector2 position_ = { 0.0f, 0.0f };

	// �X�v���C�g
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// �R���C�_�[
	std::unique_ptr<BoxCollider> collider_ = nullptr;

	// �ړ�����
	Vector2 velocity_ = { 0.0f, 0.0f };

	// �����t���O
	bool isAlive_ = true;
#pragma endregion

#pragma region �����o�֐�
public:
	// �f�X�g���N�^
	~PlayerBullet();

	// ����������
	void Initialize(const Vector2& pos, const Vector2& velocity);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�����
	void OnCollision(uint16_t damage);

	// �s��X�V
	void MatUpdate();
#pragma endregion

#pragma region �Z�b�^�[�֐�
	// �e�N�X�`���n���h����ݒ�
	static void SetHandle(uint16_t handle) { sHandle_ = handle; }

	// �{�X��ݒ�
	static void SetBoss(Boss* boss) { sBoss_ = boss; }

	// �Փ˃}�l�[�W���[2D��ݒ�
	static void SetColMgr2D(CollisionManager2D* colMgr2D) { sColMgr2D_ = colMgr2D; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �����t���O���擾
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};