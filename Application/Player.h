#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"
#include"Key.h"
#include"BoxCollider.h"
#include"CollisionManager2D.h"
#include"PlayerBullet.h"

class Player
{
public:

	//������
	void Initialize(uint16_t playerTexture, const Vector2& pos);

	//�X�V
	void Update();

	//�s��̍X�V
	void MatUpdate();

	//�`��
	void Draw();

	//�����蔻��R�[���o�b�N
	void OnCollision();

	//�_���[�W���󂯂�
	void Damage(uint16_t damageValue);

	//�u���b�N�̒ǉ�
	void AddBlock(Block* block);

	Vector2 GetPosition()const { return position; }

	float GetRotation()const { return rotation; }

	void DrawUI();

	int GetLevel()const { return level; }

private:

	//�ړ�
	void Move();

	//��]
	void Rotate();

	//�U��
	void Attack();

	//�f�o�b�O�p�֐�
	void AddBlock();

	//�e�̍X�V
	void BulletsUpdate();

	//�u���b�N�z��̍X�V
	void UpdateBlocks();

	//�u���b�N�̃I�t�Z�b�g�̍X�V
	void UpdateOffset();

	//�u���b�N���Z�b�g
	void BlockReset();

	//���x���̍X�V
	void LevelUpdate();

	void UpdateUI();

private:

	Pad* pad = nullptr;

	Key* key = nullptr;


	//���W
	Vector2 position{0,0};
	Vector2 oldPos{ 0,0 };

	//�ړ��֌W
	const float baseSpd = 32.0f;
	int moveCoolTimeMax = 2;
	int moveCoolTime = moveCoolTimeMax;
	bool isMoveHorizontal = false;

	//Vector2 moveTileMin{ 0,3 };
	//Vector2 moveTileMax {}

	//��]�p
	float rotation = 0;
	float beforeRot = 0;
	float afterRot = 0;
	float childRotation = 0;

	//��]�̃C�[�W���O�p�p�����[�^
	const float easeTimeMax = 15;
	float rotEaseTime = easeTimeMax;

	//HP�֌W
	int healthMax = 10;
	int health = healthMax;
	float hpBarMax = 0.0f;
	std::unique_ptr<Sprite> spriteHpBar = nullptr;
	uint16_t texHpBar = 0;
	std::unique_ptr<Sprite> spriteHpFrame = nullptr;
	uint16_t texHpFrame = 0;
	std::unique_ptr<Sprite> spriteHpText = nullptr;
	uint16_t texHpText = 0;

	//�ˌ��p�p�����[�^
	int shotCooltimeMax = 30;
	int shotCooltime = shotCooltimeMax;
	std::list < std::unique_ptr<PlayerBullet>> bullets;

	//���x���֌W�p�����[�^
	int needEXP = 32;
	int currentEXP = 0;
	int countBoostEXP = 4;
	int boostRateEXP = 2;

	int level = 1;
	int bulletDamage = 1;
	float expBarMax = 0.0f;
	std::unique_ptr<Sprite> spriteExpBar = nullptr;
	uint16_t texExpBar = 0;
	std::unique_ptr<Sprite> spriteExpFrame = nullptr;
	uint16_t texExpFrame = 0;
	std::unique_ptr<Sprite> spriteExpText = nullptr;
	uint16_t texExpText = 0;

	//�f�o�b�O�p
	int debugBlockOffsetX = 0;
	int debugBlockOffsetY = 0;

	//�����̃u���b�N1
	Block* playerBlock = nullptr;

	//�u���b�N�z��
	std::vector < Block* > blocks;

};

