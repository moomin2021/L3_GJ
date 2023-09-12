#pragma once
#include"Block.h"
#include<vector>

class Player;

class Piece
{
public:

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	static void CreatePiece();

	static void ALlPieceUpdate();

	void OnCollision();

public:

	//全ピース
	static std::vector < std::unique_ptr<Piece>> pieces;

	//進行速度
	static int moveCoolTimeMax;
	static int moveCoolTime;
	static float baseSpd;

private:


	//親のブロック
	Vector2 parentPos;

	//子ブロック配列
	std::vector<Block*> childBlocks;
	uint16_t myTag = 0;





	//回転角
	float rotation = 0;

};

