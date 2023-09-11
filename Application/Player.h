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

	//�u���b�N�̒ǉ�
	void AddBlock(Block* block);

	Vector2 GetPosition()const { return position; }

	float GetRotation()const { return rotation; }

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

private:

	Pad* pad = nullptr;

	Key* key = nullptr;

	//�{�̂̃X�v���C�g
	std::unique_ptr<Sprite> sprite;

	//���W
	Vector2 position{0,0};
	Vector2 oldPos{ 0,0 };

	//�e�N�X�`��
	uint16_t texIndex = 0;

	//�ړ��֌W
	const float baseSpd = 32.0f;
	int moveCoolTimeMax = 2;
	int moveCoolTime = moveCoolTimeMax;


	//��]�p
	float rotation = 0;
	float beforeRot = 0;
	float afterRot = 0;
	float childRotation = 0;

	//��]�̃C�[�W���O�p�p�����[�^
	const float easeTimeMax = 15;
	float rotEaseTime = easeTimeMax;

	int healthMax = 10;
	int health = healthMax;

	//�ˌ��p�p�����[�^
	int shotCooltimeMax = 30;
	int shotCooltime = shotCooltimeMax;
	std::list < std::unique_ptr<PlayerBullet>> bullets;

	//���x���֌W�p�����[�^
	int currentEXP = 0;
	int level = 1;

	//�f�o�b�O�p
	int debugBlockOffsetX = 0;
	int debugBlockOffsetY = 0;

	//�u���b�N�z��
	std::vector < Block* > blocks;

	//�}�l�[�W��
	CollisionManager2D* colManager = nullptr;
	//�R���C�_�[
	std::unique_ptr<BoxCollider> collider = nullptr;

};

