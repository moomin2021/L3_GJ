#pragma once
#include"Block.h"
#include<vector>


class Piece
{
public:

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();


private:

	//親のブロック
	Vector2 parentPos;

	//子ブロック配列
	std::vector<std::unique_ptr<Block>> childBlocks;

	//進行速度
	const float baseSpd = 16.0f;

	//回転角
	float rotation = 0;

};

