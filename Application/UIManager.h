#pragma once
#include "Sprite.h"

#include <memory>

class UIManager
{
#pragma region �����o�ϐ�
private:
	// �X�v���C�g
	std::unique_ptr<Sprite> sFrame_ = nullptr;
	std::unique_ptr<Sprite> sExp_ = nullptr;
	std::unique_ptr<Sprite> sExpIn_ = nullptr;
	std::unique_ptr<Sprite> sExpText_ = nullptr;
	std::unique_ptr<Sprite> sHpPlayer_ = nullptr;
	std::unique_ptr<Sprite> sHpPlayerIn_ = nullptr;
	std::unique_ptr<Sprite> sHpTextPlayer_ = nullptr;
	std::unique_ptr<Sprite> sHpBoss_ = nullptr;
	std::unique_ptr<Sprite> sHpBossIn_ = nullptr;
	std::unique_ptr<Sprite> sHpTextBoss_ = nullptr;
	std::unique_ptr<Sprite> sKakeru2_ = nullptr;
	std::unique_ptr<Sprite> sNextText_ = nullptr;

	// �摜�n���h��
	uint16_t hFrame_ = 0;
	uint16_t hExp_ = 0;
	uint16_t hExpIn_ = 0;
	uint16_t hExpText_ = 0;
	uint16_t hHpPlayer_ = 0;
	uint16_t hHpPlayerIn_ = 0;
	uint16_t hHpTextPlayer_ = 0;
	uint16_t hHpBoss_ = 0;
	uint16_t hHpBossIn_ = 0;
	uint16_t hHpTextBoss_ = 0;
	uint16_t hKakeru2_ = 0;
	uint16_t hNextText_ = 0;
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