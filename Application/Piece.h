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

	void OnCollision();

public:

	//�S�s�[�X
	static std::vector < std::unique_ptr<Piece>> pieces;

private:

	//�s�[�X���ʃ^�O
	uint16_t pieceTag = 0;

	//�e�̃u���b�N
	Vector2 parentPos;

	//�q�u���b�N�z��
	std::vector<Block*> childBlocks;

	//�i�s���x
	const float baseSpd = 16.0f;

	//��]�p
	float rotation = 0;

};

