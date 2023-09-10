#pragma once
#include "Sprite.h"

#include <memory>

class UIManager
{
#pragma region �����o�ϐ�
private:
	// �X�v���C�g
	std::unique_ptr<Sprite> sFrame_ = nullptr;
	std::unique_ptr<Sprite> sPlayerHpIn_ = nullptr;

	// �摜�n���h��
	uint16_t hFrame_ = 0;
	uint16_t hPlayerHpIn_ = 0;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	UIManager();

	// �f�X�g���N�^
	~UIManager();

	// ����������
	void Initialize();

	// �s��X�V����
	void MatUpdate();

	// �`�揈��
	void Draw();
#pragma endregion
};