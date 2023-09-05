#pragma once
#include"Texture.h"
#include"Sprite.h"

struct ParentData {
	Vector2 parentPos;
	Vector2 tileOffset;
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

public://メンバ関数
	void Initialize(const BlockData& blockData,const ParentData& parent);

	void SetParent(const ParentData& parent);

	virtual void Draw();

private://静的メンバ変数
	static uint16_t cannonTexture;
	static uint16_t blockTexture;
	static Vector2 blockSize;

private://メンバ変数

	//TODO:コライダーの定義

	Sprite sprite;
	BlockData blockData = BlockData::None;
	ParentData parent;



};

