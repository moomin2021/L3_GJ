#pragma once
#include "BaseScene.h"
#include "Key.h"

#include "Camera.h"
#include "Sprite.h"

#include <memory>

class GameScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Key* key_ = nullptr;// �L�[�{�[�h����

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	GameScene();

	// �f�X�g���N�^
	~GameScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �Փˎ�����
	void OnCollision();

	// �I�u�W�F�N�g�X�V����
	void ObjUpdate();
#pragma endregion
};