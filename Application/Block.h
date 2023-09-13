#pragma once
#include"Texture.h"
#include"Sprite.h"
#include<memory>
#include"CircleCollider.h"
#include"BoxCollider.h"
#include"CollisionManager2D.h"
#include<list>
#include"ParticleEmitter2D.h"

struct ParentData {
	Vector2 parentPos{ 0,0 };
	float parentRot = 0.0f;
	Vector2 tileOffset{ 0,0 };
	uint16_t parentTag = 0;
};

enum class BlockData {
	Player,	//プレイヤー
	None,	//通常ブロック
	Cannon	//大砲

};

class Player;

class Piece;

class Block
{
public://静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="cannonTex">大砲のテクスチャ</param>
	/// <param name="blockTex">ブロックのテクスチャ</param>
	/// <param name="playerTex">プレイヤーのテクスチャ</param>
	/// <param name="blockSize">ブロックの大きさ</param>
	static void StaticInitialize(uint16_t cannonTex, uint16_t blockTex, uint16_t playerTex, const Vector2& blockSize);

	static Vector2 GetBlockSize() { return blockSize; }

	static Block* CreateBlock(const BlockData& blockData, ParentData* parent);

	static void SetPlayer(Player* player);
	static void SetPiece(std::vector < std::unique_ptr<Piece>>* pieces);

	static size_t GetBlockCount() { return pAllBlock.size(); }

	static void AllBlockDeleteCheck();

	static void DrawParticle();

public://メンバ関数


	~Block();

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

	BlockData GetBlockData() const { return blockData; }

	void OffsetUpdate();

	Vector2 GetPosition()const { return sprite->GetPosition(); }

	void SetAlive(bool alive) { isAlive = alive; }

	void SetColor(const float4 c) { color = c; }

	BoxCollider* GetCollider()const { return collider.get(); }

private://静的メンバ変数
	static uint16_t cannonTexture;
	static uint16_t blockTexture;
	static uint16_t playerTexture;
	static Vector2 blockSize;
	//全てのブロックのﾎﾟｲﾝﾀ
	static std::vector<std::unique_ptr<Block>> pAllBlock;

	static Player* player;
	static std::vector < std::unique_ptr<Piece>>* pieces;

	static uint16_t allBlockCount;

	static std::unique_ptr<ParticleEmitter2D> particle;

private://メンバ変数

	//マネージャ
	CollisionManager2D* colManager = nullptr;
	//コライダー
	std::unique_ptr<BoxCollider> collider = nullptr;

	Vector2 currentOffset{ 0,0 };

	Vector2 oldPos{ 0,0 };

	float4 color = { 1,1,1,1 };

	std::unique_ptr<Sprite> sprite = nullptr;
	BlockData blockData = BlockData::None;
	ParentData* parent = nullptr;

	uint16_t colliderTag = 0;

	bool isAlive = false;



private: //静的メンバ関数



	void ChangeParent(uint16_t baseBlockTag, uint16_t hitBlockTag, uint16_t parentTag, const Vector2& hitOffset);
};

