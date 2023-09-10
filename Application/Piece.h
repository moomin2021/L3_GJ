#pragma once
#include"Block.h"
#include<vector>

class Piece {
#pragma region メンバ変数
private:
	// ブロック
	std::vector <Block*> blocks_;

	// 座標
	Vector2 position_ = { 0.0f, 0.0f };

	// 移動速度
	float moveSpd_ = 16.0f;

	// 生存フラグ
	bool isAlive_ = true;

	// 衝突フラグ
	bool isCol_ = false;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Piece();

	// デストラクタ
	~Piece();

	// 初期化処理
	void Initialize(const Vector2& pos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// ブロック追加
	void AddBlock(Block* block) { blocks_.emplace_back(block); }
#pragma endregion

#pragma region セッター関数
	// 生存フラグを設定
	void SetIsAlive(bool frag) { isAlive_ = frag; }

	// 衝突フラグを設定
	void SetIsCol(bool frag) { isCol_ = frag; }
#pragma endregion

#pragma region ゲッター関数
	// 生存フラグを取得
	bool GetIsAlive() { return isAlive_; }

	// 衝突フラグを取得
	bool GetIsCol() { return isCol_; }
#pragma endregion
};