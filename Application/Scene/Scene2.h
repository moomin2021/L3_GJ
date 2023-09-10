#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Camera.h"
#include "Sprite.h"
#include "BoxCollider.h"

#include <memory>

class Scene2 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> sprite_[2];

	// �R���C�_�[
	std::unique_ptr<BoxCollider> collider_[2];

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene2();

	// �f�X�g���N�^
	~Scene2();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �Փˎ�����
	void OnCollision();

	// �s��X�V
	void MatUpdate();
};

