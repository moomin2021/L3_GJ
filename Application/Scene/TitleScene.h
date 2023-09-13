#pragma once
#include "BaseScene.h"
#include "Pad.h"
#include "Camera.h"
#include "Sprite.h"
#include "Fade.h"
#include "Key.h"
#include "Sound.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Pad* pad_ = nullptr;
	Key* key_ = nullptr;
	Sound* sound_ = nullptr;

	// �t�F�[�h
	std::unique_ptr<Fade> fade_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> sBackGround_ = nullptr;
	std::unique_ptr<Sprite> sTitleFrame_ = nullptr;
	std::unique_ptr<Sprite> sTitleLogo_ = nullptr;
	std::unique_ptr<Sprite> sStartText_ = nullptr;
	std::unique_ptr<Sprite> sExitText_ = nullptr;
	std::unique_ptr<Sprite> sTitleSelectFrame_ = nullptr;;

	// �摜�n���h��
	uint16_t hBackGround_ = 0;
	uint16_t hTitleFrame_ = 0;
	uint16_t hTitleLogo_ = 0;
	uint16_t hStartText_ = 0;
	uint16_t hExitText_ = 0;
	uint16_t hTitleSelectFrame_ = 0;

	// �T�E���h�n���h��
	uint16_t sTitleBGM_ = 0;
	uint16_t selectSE_ = 0;
	uint16_t selectMoveSE_ = 0;

	// �I�������W
	std::vector<Vector2> selectPos_ = {
		{ 628.0f, 740.0f },
		{ 1228.0f, 740.0f },
	};

	// �I�𐔎�
	INT16 selectNum_ = 0;

	// L�X�e�B�b�NX���W�ۑ�
	float oldLStickX= 0.0f;

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

