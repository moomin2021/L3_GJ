#pragma once
#include "Sprite.h"

#include <memory>

class Boss;

class UIManager
{
#pragma region �����o�ϐ�
private:
	// �{�X
	static Boss* sBoss_;

	// �X�v���C�g
	std::unique_ptr<Sprite> sTitleFrame_ = nullptr;
	std::unique_ptr<Sprite> sExp_ = nullptr;
	std::unique_ptr<Sprite> sExpIn_ = nullptr;
	std::unique_ptr<Sprite> sExpText_ = nullptr;
	std::unique_ptr<Sprite> sHpPlayer_ = nullptr;
	std::unique_ptr<Sprite> sHpPlayerIn_ = nullptr;
	std::unique_ptr<Sprite> sHpTextPlayer_ = nullptr;
	std::unique_ptr<Sprite> sHpBoss_ = nullptr;
	std::unique_ptr<Sprite> sHpTextBoss_ = nullptr;
	std::unique_ptr<Sprite> sNextText_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sTimes_ = {};
	std::unique_ptr<Sprite> sColon_ = nullptr;

	// �摜�n���h��
	uint16_t hTitleFrame_ = 0;
	uint16_t hExp_ = 0;
	uint16_t hExpIn_ = 0;
	uint16_t hExpText_ = 0;
	uint16_t hHpPlayer_ = 0;
	uint16_t hHpPlayerIn_ = 0;
	uint16_t hHpTextPlayer_ = 0;
	uint16_t hHpBoss_ = 0;
	uint16_t hHpTextBoss_ = 0;
	uint16_t hNextText_ = 0;
	std::vector<uint16_t> hNumbers_ = {};
	uint16_t hColon_ = 0;

	// �Q�[���o�ߎ���
	uint64_t startGameTime_ = 0;
	std::vector<uint16_t> gameTimes_ = {};
	std::vector<uint16_t> clearTime_ = {};

	// �N���A�t���O
	bool isClear_ = false;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	UIManager();

	// �f�X�g���N�^
	~UIManager();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �s��X�V����
	void MatUpdate();

	// �`�揈��
	void Draw();
#pragma endregion

#pragma region �Z�b�^�[�֐�
	// �{�X��ݒ�
	static void SetBoss(Boss* boss) { sBoss_ = boss; }
#pragma endregion
};