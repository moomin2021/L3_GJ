#pragma once
#include"Block.h"
#include<vector>

struct PieceData {
	std::vector<Vector2> offset;
	std::vector<BlockData> blockData;
	uint16_t previewTexture = 0;
	Vector2 textureSize{ 0,0 };
	float rotation = 0.0f;
};

class Player;

class Piece
{
public:

	//������
	void Initialize(const PieceData& data);

	//�X�V
	void Update();

	//�`��
	void Draw();

	static void ALlPieceUpdate();

	void OnCollision();

	size_t GetChildCount()const { return childBlocks.size(); }

public:

	//�i�s���x
	static int moveCoolTimeMax;
	static int moveCoolTime;
	static float baseSpd;

	static uint16_t allPieceCount;

private:


	//�e�̃u���b�N
	Vector2 parentPos;

	//�q�u���b�N�z��
	std::vector<Block*> childBlocks;
	uint16_t myTag = 0;





	//��]�p
	float rotation = 0;

};

