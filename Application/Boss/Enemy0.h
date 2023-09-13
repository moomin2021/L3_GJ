#pragma once
#include "Sprite.h"
#include "CollisionManager2D.h"
#include "CircleCollider.h"

#include <memory>

class Enemy0
{
#pragma region �����o�ϐ�
private:
	// �摜�n���h��
	static uint16_t hBack_;
	static uint16_t hFront_;
	static CollisionManager2D* sColMgr2D_;

	// �X�v���C�g
	std::unique_ptr<Sprite> sBack_ = nullptr;// ����
	std::unique_ptr<Sprite> sFront_ = nullptr;// �\��

	// �R���C�_�[
	std::unique_ptr<CircleCollider> collider_ = nullptr;

	// �����t���O
	bool isAlive_ = true;

	// ���W
	Vector2 position_ = { 0.0f, 0.0f };

	// ��]
	float rotate_ = 0.0f;

	// ���x
	float speed_ = 10.0f;

	// ��]���x
	float rotateSpd_ = 3.0f;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Enemy0();

	// �f�X�g���N�^
	~Enemy0();

	// ����������
	void Initialize(Vector2 pos);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�����
	void OnCollision();

	// �s��X�V����
	void MatUpdate();
#pragma endregion

#pragma region �Z�b�^�[�֐�
	// ���ʃn���h����ݒ�
	static void SetBackHandle(uint16_t handle) { hBack_ = handle; }

	// �\�ʃn���h����ݒ�
	static void SetFrontHandle(uint16_t handle) { hFront_ = handle; }

	// �Փ˃}�l�[�W���[2D��ݒ�
	static void SetColMgr2D(CollisionManager2D* colMgr2D) { sColMgr2D_ = colMgr2D; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �����t���O���擾
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};