#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Mouse.h"
#include "PointCollider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Sprite.h"
#include "CollisionManager2D.h"

#include <memory>

class Scene3 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;

	CollisionManager2D* colMgr2D_ = nullptr;

	std::unique_ptr<PointCollider> pCol_ = nullptr;
	std::unique_ptr<BoxCollider> boxCol_ = nullptr;
	std::unique_ptr<CircleCollider> cirCol_ = nullptr;

	std::unique_ptr<Sprite> sBox_ = nullptr;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene3();

	// �f�X�g���N�^
	~Scene3();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �Փ˔���㏈��
	void OnCollision();

	// �I�u�W�F�N�g�X�V
	void ObjUpdate();
};

