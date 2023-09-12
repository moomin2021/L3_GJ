#pragma once
#include "BaseScene.h"
#include "Pad.h"
#include "Key.h"
#include "Camera.h"
#include "Sprite.h"
#include "Fade.h"

#include <memory>

class ResultScene : public BaseScene
{
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	Pad* pad_ = nullptr;
	Key* key_ = nullptr;

	// �t�F�[�h
	std::unique_ptr<Fade> fade_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �X�v���C�g
	std::unique_ptr<Sprite> sBackGround_ = nullptr;
	std::unique_ptr<Sprite> sFrame_ = nullptr;
	std::unique_ptr<Sprite> sResultLogo_ = nullptr;
	std::unique_ptr<Sprite> sTimeText_ = nullptr;
	std::unique_ptr<Sprite> sRetryText_ = nullptr;
	std::unique_ptr<Sprite> sTitleText_ = nullptr;
	std::unique_ptr<Sprite> sRankPiece_ = nullptr;
	std::unique_ptr<Sprite> sRankText_ = nullptr;
	std::unique_ptr<Sprite> sNextRankText_ = nullptr;
	std::unique_ptr<Sprite> sResultSelectFrame_ = nullptr;

	// �摜�n���h��
	uint16_t hBackGround_ = 0;
	uint16_t hFrame_ = 0;
	uint16_t hResultLogo_ = 0;
	uint16_t hTimeText_ = 0;
	uint16_t hRetryText_ = 0;
	uint16_t hTitleText_ = 0;
	uint16_t hRankPiece_ = 0;
	uint16_t hRankText_ = 0;
	uint16_t hNextRankText_ = 0;
	uint16_t hResultSelectFrame_ = 0;

	// �I�������W
	std::vector<Vector2> selectPos_ = {
		{ 464.0f, 624.0f },
		{ 464.0f, 752.0f },
	};

	// �I�𐔎�
	INT16 selectNum_ = 0;

	// L�X�e�B�b�NY���W�ۑ�
	float oldLStickY = 0.0f;

#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	ResultScene();

	// �f�X�g���N�^
	~ResultScene();

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

