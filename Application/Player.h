#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"
#include"CircleCollider.h"
#include"CollisionManager2D.h"

class Player
{
public:

	//������
	void Initialize(uint16_t playerTexture, const Vector2& pos);

	//�X�V
	void Update();

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

	void Rotate();


	//�f�o�b�O�p�֐�
	void AddBlock();

	

	//�u���b�N�z��̍X�V
	void UpdateBlocks();

private:

	Pad* pad = nullptr;



	//�{�̂̃X�v���C�g
	std::unique_ptr<Sprite> sprite;

	//���W
	Vector2 position{0,0};

	//�e�N�X�`��
	uint16_t texIndex = 0;

	//���x
	const float baseSpd = 8.0f;

	//��]�p
	float rotation = 0;
	float beforeRot = 0;
	float afterRot = 0;


	const float easeTimeMax = 15;
	float rotEaseTime = easeTimeMax;

	//�f�o�b�O�p
	int debugBlockOffsetX = 0;
	int debugBlockOffsetY = 0;

	//�u���b�N�z��
	std::vector < Block* > blocks;

	//�}�l�[�W��
	CollisionManager2D* colManager = nullptr;
	//�R���C�_�[
	std::unique_ptr<CircleCollider> collider = nullptr;

};

