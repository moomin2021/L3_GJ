#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"

//�u���b�N�N���X�p��
class Player
{
public:

	//������
	void Initialize(uint16_t playerTexture);

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
	int rotation = 0;

	//�u���b�N�z��
	std::vector<Block> blocks;

};

