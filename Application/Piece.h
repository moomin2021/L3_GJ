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

	void OnCollision();

public:

	//全ピース
	static std::vector < std::unique_ptr<Piece>> pieces;

private:

	//ピース識別タグ
	uint16_t pieceTag = 0;

	//親のブロック
	Vector2 parentPos;

	//子ブロック配列
	std::vector<Block*> childBlocks;

	//進行速度
	const float baseSpd = 16.0f;

	//回転角
	float rotation = 0;

};

