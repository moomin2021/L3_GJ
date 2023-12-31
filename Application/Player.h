#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"
#include"Key.h"
#include"BoxCollider.h"
#include"CollisionManager2D.h"
#include"PlayerBullet.h"
#include"Sound.h"
#include"ParticleEmitter2D.h"

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

	//ダメージを受ける
	void Damage(uint16_t damageValue);

	//ブロックの追加
	void AddBlock(Block* block);

	Vector2 GetPosition()const { return position; }

	float GetRotation()const { return rotation; }

	void DrawUI();

	int GetLevel()const { return level; }

	bool IsRotate()const { return isRotate; }

	bool IsAlive()const { return isAlive; };

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

	//レベルの更新
	void LevelUpdate();

	void UpdateUI();

private:

	Pad* pad = nullptr;

	Key* key = nullptr;

	Sound* sound = nullptr;

	//座標
	Vector2 position{0,0};
	Vector2 oldPos{ 0,0 };

	//移動関係
	const float baseSpd = 32.0f;
	int moveCoolTimeMax = 2;
	int moveCoolTime = moveCoolTimeMax;
	bool isMoveHorizontal = false;

	//Vector2 moveTileMin{ 0,3 };
	//Vector2 moveTileMax {}

	//回転角
	float rotation = 0;
	float beforeRot = 0;
	float afterRot = 0;
	float childRotation = 0;
	bool isRotate = false;
	std::unique_ptr<Sprite> spriteRotUI = nullptr;
	uint16_t texRotUI = 0;
	int timerDrawRotUIMax = 60 * 60;
	int timerDrawRotUI = timerDrawRotUIMax;

	//回転のイージング用パラメータ
	const float easeTimeMax = 15;
	float rotEaseTime = easeTimeMax;

	//HP関係
	int healthMax = 8;
	int health = healthMax;
	float hpBarMax = 0.0f;
	std::unique_ptr<Sprite> spriteHpBar = nullptr;
	uint16_t texHpBar = 0;
	std::unique_ptr<Sprite> spriteHpFrame = nullptr;
	uint16_t texHpFrame = 0;
	std::unique_ptr<Sprite> spriteHpText = nullptr;
	uint16_t texHpText = 0;
	int damageCoolTimeMax = 90;
	int damageCooltime = 0;
	bool isAlive = false;

	//射撃用パラメータ
	int shotCooltimeMax = 30;
	int shotCooltime = shotCooltimeMax;
	std::list < std::unique_ptr<PlayerBullet>> bullets;

	//レベル関係パラメータ
	int needEXP = 40;
	int currentEXP = 0;
	int countBoostEXP = 20;
	int boostRateEXP = 3;

	int level = 1;
	int bulletDamage = 1;
	float expBarMax = 0.0f;
	std::unique_ptr<Sprite> spriteExpBar = nullptr;
	uint16_t texExpBar = 0;
	std::unique_ptr<Sprite> spriteExpFrame = nullptr;
	uint16_t texExpFrame = 0;
	std::unique_ptr<Sprite> spriteExpText = nullptr;
	uint16_t texExpText = 0;
	bool isDrawPressA = false;
	std::unique_ptr<Sprite> spritePressA = nullptr;
	uint16_t texPressA = 0;
	float sizeButtonA = 48.0f;

	//デバッグ用
	int debugBlockOffsetX = 0;
	int debugBlockOffsetY = 0;

	uint16_t soundRot = 0;
	uint16_t soundAtk = 0;
	uint16_t soundDmg = 0;
	uint16_t soundStick = 0;
	uint16_t soundEXP = 0;

	int addSoundCount = 0;

	//2Dエミッター
	std::unique_ptr<ParticleEmitter2D> particle = nullptr;

	//色
	float4 myColor;

	//自分のブロック1
	Block* playerBlock = nullptr;

	//ブロック配列
	std::vector < Block* > blocks;

};

