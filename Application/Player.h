#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"
#include"Key.h"
#include"BoxCollider.h"
#include"CollisionManager2D.h"
#include"PlayerBullet.h"

class Player
{
public:

	//初期化
	void Initialize(uint16_t playerTexture, const Vector2& pos);

	//更新
	void Update();

	//行列の更新
	void MatUpdate();

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

	//回転
	void Rotate();

	//攻撃
	void Attack();

	//デバッグ用関数
	void AddBlock();

	//弾の更新
	void BulletsUpdate();

	//ブロック配列の更新
	void UpdateBlocks();

	//ブロックのオフセットの更新
	void UpdateOffset();

	//ブロックリセット
	void BlockReset();

private:

	Pad* pad = nullptr;

	Key* key = nullptr;

	//本体のスプライト
	std::unique_ptr<Sprite> sprite;

	//座標
	Vector2 position{0,0};
	Vector2 oldPos{ 0,0 };

	//テクスチャ
	uint16_t texIndex = 0;

	//移動関係
	const float baseSpd = 32.0f;
	int moveCoolTimeMax = 2;
	int moveCoolTime = moveCoolTimeMax;


	//回転角
	float rotation = 0;
	float beforeRot = 0;
	float afterRot = 0;
	float childRotation = 0;

	//回転のイージング用パラメータ
	const float easeTimeMax = 15;
	float rotEaseTime = easeTimeMax;

	int healthMax = 10;
	int health = healthMax;

	//射撃用パラメータ
	int shotCooltimeMax = 30;
	int shotCooltime = shotCooltimeMax;
	std::list < std::unique_ptr<PlayerBullet>> bullets;

	//レベル関係パラメータ
	int currentEXP = 0;
	int level = 1;

	//デバッグ用
	int debugBlockOffsetX = 0;
	int debugBlockOffsetY = 0;

	//ブロック配列
	std::vector < Block* > blocks;

	//マネージャ
	CollisionManager2D* colManager = nullptr;
	//コライダー
	std::unique_ptr<BoxCollider> collider = nullptr;

};

