#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"

//�u���b�N�N���X�p��
class Player
{
public:

	//������
	void Initialize(uint16_t playerTexture,const Vector2& pos);

	//�X�V
	void Update();

	//�`��
	void Draw();

private:

	//�ړ�
	void Move();

	void Rotate();


	//�f�o�b�O�p�֐�
	void AddBlock();


private:

	Pad* pad = nullptr;

	//�{�̂̃X�v���C�g
	std::unique_ptr<Sprite> sprite;

	//�e�N�X�`��
	uint16_t texIndex = 0;

	//���x
	const float baseSpd = 2.0f;

	//��]�p
	float rotation = 0;
	float beforeRot = 0;
	float afterRot = 0;


	const float easeTimeMax = 15;
	float rotEaseTime = easeTimeMax;


	//�u���b�N�z��
	std::vector<Block> blocks;

};

