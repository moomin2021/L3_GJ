#pragma once
#include"Block.h"
#include<vector>

class Player;

class Piece
{
public:

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	static void CreatePiece();

	static void ALlPieceUpdate();

	void OnCollision();

public:

	//�S�s�[�X
	static std::vector < std::unique_ptr<Piece>> pieces;

	//�i�s���x
	static int moveCoolTimeMax;
	static int moveCoolTime;
	static float baseSpd;

private:


	//�e�̃u���b�N
	Vector2 parentPos;

	//�q�u���b�N�z��
	std::vector<Block*> childBlocks;
	uint16_t myTag = 0;





	//��]�p
	float rotation = 0;

};

