#pragma once
#include "Sprite.h"
#include "SceneManager.h"

#include <memory>

class Fade
{
#pragma region �����o�ϐ�
private:
	// �X�v���C�g
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// �摜�n���h��
	uint16_t handle_ = 0;

	// �t�F�[�h�C���A�E�g�t���O
	static bool isFade_;
	static bool isFadeIn_;
	static bool isFadeOut_;

	float fadeInTime_ = 1.0f;
	float fadeOutTime_ = 1.0f;
	uint64_t startTime_ = 0;

	static SCENE nextScene_;
#pragma endregion

#pragma region �����o�֐�
public:
	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �s��X�V
	void MatUpdate();

	// �V�[���؂�ւ�
	void ChangeScene(SCENE sceneNum);
#pragma endregion
};