#pragma once
#include "Block.h"
#include "Piece.h"

#include <memory>
#include <vector>

class BlockManager
{
#pragma region メンバ変数
private:
	// ブロック
	std::vector<std::unique_ptr<Block>> blocks_ = {};
#pragma endregion

#pragma region メンバ関数
public:
	// インスタンス取得
	static BlockManager* GetInstance();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新
	void MatUpdate();

	// ブロックを作成
	Block* CreateBlock(const Vector2& offset, Piece* piece);

private:
	// コンストラクタ
	BlockManager();

	// デストラクタ
	~BlockManager();
#pragma endregion

#pragma region 特殊関数
	BlockManager(const BlockManager&) = delete;				// コピーコンストラクタ禁止
	BlockManager& operator = (const BlockManager&) = delete;	// コピー代入演算子禁止
#pragma endregion
};