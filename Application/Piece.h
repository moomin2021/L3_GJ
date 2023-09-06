#pragma once
#include"Block.h"
#include<vector>


class Piece
{
public:

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();


private:

	//�e�̃u���b�N
	Vector2 parentPos;

	//�q�u���b�N�z��
	std::vector<std::unique_ptr<Block>> childBlocks;

	//�i�s���x
	const float baseSpd = 16.0f;

	//��]�p
	float rotation = 0;

};

