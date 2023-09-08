#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"
#include"CircleCollider.h"
#include"CollisionManager2D.h"

class Player
{
public:

	//初期化
	void Initialize(uint16_t playerTexture, const Vector2& pos);

	//更新
	void Update();

	//描画
	void Draw();

	//当たり判定コールバック
	void OnCollision();

	//ブロックの追加
	void AddBlock(Block* block);

	Vector2 GetPosition()const { return position; }

	float GetRotation()const { return rotation; }

private:

	//移動
	void Move();

	void Rotate();


	//デバッグ用関数
	void AddBlock();

	

	//ブロック配列の更新
	void UpdateBlocks();

private:

	Pad* pad = nullptr;



	//本体のスプライト
	std::unique_ptr<Sprite> sprite;

	//座標
	Vector2 position{0,0};

	//テクスチャ
	uint16_t texIndex = 0;

	//速度
	const float baseSpd = 8.0f;

	//回転角
	float rotation = 0;
	float beforeRot = 0;
	float afterRot = 0;


	const float easeTimeMax = 15;
	float rotEaseTime = easeTimeMax;

	//デバッグ用
	int debugBlockOffsetX = 0;
	int debugBlockOffsetY = 0;

	//ブロック配列
	std::vector < Block* > blocks;

	//マネージャ
	CollisionManager2D* colManager = nullptr;
	//コライダー
	std::unique_ptr<CircleCollider> collider = nullptr;

};

