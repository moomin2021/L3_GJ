#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"

//ブロッククラス継承
class Player
{
public:

	//初期化
	void Initialize(uint16_t playerTexture,const Vector2& pos);

	//更新
	void Update();

	//描画
	void Draw();

private:

	//移動
	void Move();

	void Rotate();


	//デバッグ用関数
	void AddBlock();


private:

	Pad* pad = nullptr;

	//本体のスプライト
	std::unique_ptr<Sprite> sprite;

	//テクスチャ
	uint16_t texIndex = 0;

	//速度
	const float baseSpd = 2.0f;

	//回転角
	float rotation = 0;
	float beforeRot = 0;
	float afterRot = 0;


	const float easeTimeMax = 15;
	float rotEaseTime = easeTimeMax;


	//ブロック配列
	std::vector<Block> blocks;

};

