#pragma once
#include "Object3D.h"
#include "Key.h"
#include "CollisionManager.h"
#include "MeshCollider.h"

#include <memory>

class TestEnemy
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

	// ���R���C�_�[
	std::unique_ptr<MeshCollider> collider_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	TestEnemy();

	// �f�X�g���N�^
	~TestEnemy();

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