#pragma once
#include "BaseScene.h"
#include "Pad.h"
#include "Camera.h"
#include "Sprite.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Pad* pad_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> sBackGround_ = nullptr;

	// �摜�n���h��
	uint16_t hBackGround_ = 0;

#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �Փˎ�����
	void OnCollision();

	// �s��X�V����
	void MatUpdate();
#pragma endregion
};

