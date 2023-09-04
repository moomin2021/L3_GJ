#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "RayCollider.h"
#include "Key.h"
#include "CollisionManager.h"

#include <memory>

class TestPlayer
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Key* key_ = nullptr;				// �L�[�{�[�h����
	CollisionManager* colMgr_ = nullptr;// �R���W�����}�l�[�W���[

	// ���f��
	std::unique_ptr<Model> model_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> object_ = nullptr;
	std::unique_ptr<Object3D> rayObj_ = nullptr;

	// ���R���C�_�[
	std::unique_ptr<RayCollider> collider_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	TestPlayer();

	// �f�X�g���N�^
	~TestPlayer();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�
	void OnCollision();

	// �I�u�W�F�N�g�X�V����
	void ObjUpdate();
#pragma endregion
};