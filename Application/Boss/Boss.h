#pragma once
#include "Sprite.h"
#include "BossBullet.h"
#include "Enemy0.h"
#include "ParticleEmitter2D.h"
#include "Key.h"
#include "CircleCollider.h"
#include "CollisionManager2D.h"
#include "Sound.h"

#include <memory>
#include <string>
#include <vector>

class Player;

class Boss
{
#pragma region �񋓌^
private:
	enum State {
		OPENING0,		// �I�[�v�j���O0
		OPENING1,		// �I�[�v�j���O1
		OPENING2,		// �I�[�v�j���O2
		CLOSING0,		// �N���[�W���O0
		CLOSING1,		// �N���[�W���O1
		WAIT,			// �ҋ@
		PRE_MOVE_SHOT,	// �ړ������O����
		MOVE_SHOT,		// �ړ�����
		POST_MOVE_SHOT,	// �ړ������㏈��
		PRE_SUMMON,		// �����O����
		SUMMON,			// ����
		POST_SUMMON,	// �������Ə���
		PRE_BOOMERANG,	// �u�[�������O����
		BOOMERANG,		// �u�[������
		POST_BOOMERANG,	// �u�[�������㏈��
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	std::vector<std::string> stateText_ = {
		"OPENING0", "OPENING1", "OPENING2",
		"CLOSING0", "CLOSING1",
		"WAIT",
		"PRE_MOVE_SHOT", "MOVE_SHOT", "POST_MOVE_SHOT",
		"PRE_SUMMON", "SUMMON", "POST_SUMMON",
		"PRE_BOOMERANG", "BOOMERANG", "POST_BOOMERANG",
	};

	// �C���X�^���X
	Key* key_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;
	Sound* sound_ = nullptr;

	// �T�E���h�n���h��
	uint16_t bulletSound_ = 0;
	uint16_t damageSound_ = 0;
	uint16_t summonSound_ = 0;
	uint16_t deadSound_ = 0;

	// �X�v���C�g
	std::unique_ptr<Sprite> sBossBack0_ = nullptr;	// �{�X����0
	std::unique_ptr<Sprite> sBossBack1_ = nullptr;	// �{�X����1
	std::unique_ptr<Sprite> sBossFront_ = nullptr;	// �{�X�O��
	std::unique_ptr<Sprite> sHpBossIn_ = nullptr;// �{�X��HP�Q�[�W
	std::unique_ptr<Sprite> sKakeru_ = nullptr;// HP�Q�[�W�̌��\��

	// �摜�n���h��
	uint16_t hBossBack_ = 0;
	uint16_t hBossFront_ = 0;
	uint16_t hParticle0_ = 0;
	uint16_t hParticle1_ = 0;
	uint16_t hHpBossIn_ = 0;
	std::vector<uint16_t> hKakeru_ = {};

	// �R���C�_�[
	std::unique_ptr<CircleCollider> collider_ = nullptr;
	std::unique_ptr<CircleCollider> colBack0_ = nullptr;
	std::unique_ptr<CircleCollider> colBack1_ = nullptr;

	// ���
	State state_ = OPENING0;

	// HP
	size_t gaugeNum_ = 4;
	std::vector<uint16_t> hp_ = {};
	uint16_t oneGaugeValue_ = 600;

	// �����t���O
	bool isAlive_ = true;

	// ���W
	Vector2 position_ = { 1500.0f, 480.0f };// �{�X�̍��W
	Vector2 basicPos_ = { 1500.0f, 480.0f };// ��{���W
	Vector2 backPos0_ = { 1500.0f, 480.0f };// ���ʍ��W0
	Vector2 backPos1_ = { 1500.0f, 480.0f };// ���ʍ��W1

	// sin�֐��ł̈ړ����l
	float sinMove_ = 0.0f;	// sin�֐��ɓn�����l
	float sinSpd_ = 0.05f;	// sin�l�̉��Z���x

	// �p�[�e�B�N���G�~�b�^�[
	std::unique_ptr<ParticleEmitter2D> emitterBack0_ = nullptr;
	std::unique_ptr<ParticleEmitter2D> emitterBack1_ = nullptr;
	std::unique_ptr<ParticleEmitter2D> emitterFront_ = nullptr;

	// �{�X���ʂ̊p�x
	float rotateBossBack0_ = 0.0f;
	float rotateBossBack1_ = 45.0f;
	float rotateBossFront_ = 0.0f;

	// �{�X���ʂ̉�]���x
	float basicSpd_ = 3.0f;
	float moveShotRotaSpd_ = 30.0f;
	float summonRotaSpd_ = 30.0f;
	float boomerangRotaSpd_ = 30.0f;

	// �G
	std::vector<std::unique_ptr<Enemy0>> enemys_;
	float summonInterval_ = 1.0f;	// ��������
	uint64_t summonTime_ = 0;		// ������������

	// �e�֘A
	std::vector<std::unique_ptr<BossBullet>> bullets_;	// �e
	float shotInterval_ = 1.0f;							// �e�����Ԋu[s]
	uint64_t shotTime_ = 0;								// �e������������

	// �s���J�ڊ֘A
	Vector2 beforePos_ = { 0.0f, 0.0f };	// �s���J�ڑO���W
	Vector2 beforeBackPos0_ = { 0.0f, 0.0f };// ����0�s���O���W
	Vector2 beforeBackPos1_ = { 0.0f, 0.0f };// ����0�s���O���W
	uint64_t actionStartTime_= 0;	// �s���J�n����
	float time2Opening0_		= 3.0f;// �I�[�v���C���O0����
	float time2Opening1_		= 5.0f;// �I�[�v���C���O1����
	float time2Opening2_		= 1.0f;// �I�[�v���C���O2����
	float timeClosing0_			= 5.0f;// �I�[�v���C���O0����
	float timeClosing1_			= 0.5f;// �I�[�v���C���O1����
	float time2PreMoveShot_		= 3.0f;	// �ړ������O����
	float time2MoveShot_		= 10.0f;// �ړ���������
	float time2PostMoveShot_	= 3.0f;	// �ړ������㎞��
	float time2PreSummon_		= 3.0f;	// �����O����
	float time2Summon_			= 10.0f;// �������鎞��
	float time2PostSummon_		= 3.0f;	// �����㎞��
	float time2PreBoomerang_	= 3.0f;	// �����O����
	float time2Boomerang_		= 10.0f;// �������鎞��
	float time2PostBoomerang_	= 3.0f;	// �����㎞��

	bool isBoomerang_ = false;

	bool isChangeScene_ = false;

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

	// UI�`��
	void UIDraw();

	// �Փˎ�����
	void OnCollision();

	// �s��X�V
	void MatUpdate();

	// �{�X��HP����
	void SubHP(uint16_t value);

private:
	// ��ԕʍX�V����
	static void (Boss::* stateTable[]) ();

	// ��ԕʍs��
	void Opening0();		// �I�[�v�j���O0
	void Opening1();		// �I�[�v�j���O1
	void Opening2();		// �I�[�v�j���O2
	void Closing0();		// �N���[�W���O0
	void Closing1();		// �N���[�W���O1
	void Wait();			// �ҋ@
	void PreMoveShot();		// �ړ������O����
	void MoveShot();		// �ړ�����
	void PostMoveShot();	// �ړ������㏈��
	void PreSummon();		// �����O����
	void Summon();			// ����
	void PostSummon();		// �����㏈��
	void PreBoomerang();	// �u�[�������O����
	void Boomerang();		// �u�[������
	void PostBoomerang();	// �u�[�������㏈��

	void BossBackRotate(float rotate);// �{�X�̗��ʉ�]

	// �{�X��HP�X�V
	void HPUpdate();

	// ImGui���g��
	void DebugImGui();
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	bool GetIsChangeScene() { return isChangeScene_; }

	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};