#pragma once
#include"Texture.h"
#include"Sprite.h"
#include<memory>
#include"CircleCollider.h"
#include"CollisionManager2D.h"
#include<list>

struct ParentData {
	Vector2* parentPos = nullptr;
	float* parentRot = nullptr;
	Vector2 tileOffset{0,0};
	uint16_t parentTag = 0;
};

enum class BlockData {
	Player,	//プレイヤー
	None,	//通常ブロック
	Cannon	//大砲

};

class Player;

class Block
{
public://静的メンバ関数
	
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="cannonTex">大砲のテクスチャ</param>
	/// <param name="blockTex">ブロックのテクスチャ</param>
	/// <param name="blockSize">ブロックの大きさ</param>
	static void StaticInitialize(uint16_t cannonTex, uint16_t blockTex, const Vector2& blockSize);

	static Vector2 GetBlockSize() { return blockSize; }

	static Block* CreateBlock(const BlockData& blockData, ParentData* parent);

	static void SetPlayer(Player* player);

public://メンバ関数

	/// <summary>
	/// ブロックの初期化
	/// </summary>
	/// <param name="blockData">なんのブロックか</param>
	/// <param name="parent">親のデータ(座標&回転+タイルの距離)</param>
	void Initialize(const BlockData& blockData, ParentData* parent);

	void SetParent(ParentData* parent) { this->parent = parent; }

	virtual void Draw();

	void Update();

	void OnCollison();

	Vector2 GetOffset()const { return parent->tileOffset; }

	ParentData* GetParent() { return parent; }


private://静的メンバ変数
	static uint16_t cannonTexture;
	static uint16_t blockTexture;
	static Vector2 blockSize;
	//全てのブロックのﾎﾟｲﾝﾀ
	static std::vector<std::unique_ptr<Block>> pAllBlock;
	
	static Player* player;

private://メンバ変数

	//マネージャ
	CollisionManager2D* colManager = nullptr;
	//コライダー
	std::unique_ptr<CircleCollider> collider = nullptr;

	std::unique_ptr<Sprite> sprite = nullptr;
	BlockData blockData = BlockData::None;
	ParentData* parent = nullptr;

	uint16_t colliderTag = 0;

private: //静的メンバ関数

	void ChangeParent(uint16_t baseBlockTag, uint16_t hitBlockTag,uint16_t parentTag,const Vector2& hitOffset);
};

