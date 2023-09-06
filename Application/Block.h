#pragma once
#include"Texture.h"
#include"Sprite.h"
#include<memory>

struct ParentData {
	Vector2* parentPos = nullptr;
	Vector2 tileOffset{0,0};
};

enum class BlockData {
	Player,	//プレイヤー
	None,	//通常ブロック
	Cannon	//大砲

};

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

public://メンバ関数

	/// <summary>
	/// ブロックの初期化
	/// </summary>
	/// <param name="blockData">なんのブロックか</param>
	/// <param name="parent">親のデータ(座標+タイルの距離)</param>
	void Initialize(const BlockData& blockData, ParentData* parent);

	void SetParent(ParentData* parent) { this->parent = parent; }

	virtual void Draw();

	void Update();


private://静的メンバ変数
	static uint16_t cannonTexture;
	static uint16_t blockTexture;
	static Vector2 blockSize;

private://メンバ変数

	//TODO:コライダーの定義

	std::unique_ptr<Sprite> sprite = nullptr;
	BlockData blockData = BlockData::None;
	ParentData* parent = nullptr;



};

