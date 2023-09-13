#pragma once
#include"Block.h"
#include<vector>

struct PieceData {
	std::vector<Vector2> offset;
	std::vector<BlockData> blockData;
	uint16_t previewTexture = 0;
	Vector2 textureSize{ 0,0 };
};

class Player;

class Piece
{
public:

	//初期化
	void Initialize(const PieceData& data);

	//更新
	void Update();

	//描画
	void Draw();

	static void ALlPieceUpdate();

	void OnCollision();

public:

	//進行速度
	static int moveCoolTimeMax;
	static int moveCoolTime;
	static float baseSpd;

	static uint16_t allPieceCount;

private:


	//親のブロック
	Vector2 parentPos;

	//子ブロック配列
	std::vector<Block*> childBlocks;
	uint16_t myTag = 0;





	//回転角
	float rotation = 0;

};

