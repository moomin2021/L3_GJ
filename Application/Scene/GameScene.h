#pragma once
#include "BaseScene.h"

class GameScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
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