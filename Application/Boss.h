#pragma once
#include "Sprite.h"

#include <memory>

class Boss
{
#pragma region �񋓌^
private:
	enum State {
		WAIT,		// �ҋ@
		MOVE_SHOT,	// �ړ�����
		SUMMON,		// ����
		BOOMERANG,	// �u�[������
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// �X�v���C�g
	std::unique_ptr<Sprite> sBossBack0_ = nullptr;	// �{�X����0
	std::unique_ptr<Sprite> sBossBack1_ = nullptr;	// �{�X����1
	std::unique_ptr<Sprite> sBossFront_ = nullptr;	// �{�X�O��

	// �摜�n���h��
	uint16_t hBossBack_ = 0;
	uint16_t hBossFront_ = 0;

	// ���
	State state_ = WAIT;

	// �{�X�̍��W
	Vector2 position_ = { 1500.0f, 540.0f };

	// �{�X���ʂ̊p�x
	float rotateBossBack0_ = 0.0f;
	float rotateBossBack1_ = 45.0f;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Boss();

	// �f�X�g���N�^
	~Boss();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�����
	void OnCollision();

	// �s��X�V
	void MatUpdate();

private:
	// ��ԕʍX�V����
	static void (Boss::* stateTable[]) ();

	// ��ԕʍs��
	void Wait();		// �ҋ@
	void MoveShot();	// �ړ�����
	void Summon();		// ����
	void Boomerang();	// �u�[������
#pragma endregion
};