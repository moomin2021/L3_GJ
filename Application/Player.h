#pragma once
#include"Block.h"
#include<vector>
#include"Pad.h"
#include"BoxCollider.h"
#include"CollisionManager2D.h"

class Player {
#pragma region メンバ変数
private:
	// インスタンス
	Pad* pad_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// 画像ハンドル
	uint16_t handle_ = 0;

	// コライダー
	std::unique_ptr<BoxCollider> collider_ = nullptr;

	// 座標
	Vector2 position_ = { 96.0f, 540.0f };

	// 回転角
	float rotate_ = 0.0f;
	float beforeRotate_ = 0.0f;	// 回転前回転角
	float afterRotate_ = 0.0f;	// 回転後回転角

	// 移動速度
	float baseSpd_ = 8.0f;

	// 回転時間[s]
	float rotateTime_ = 0.5f;

	// 回転開始時間
	uint64_t startRotateTime_ = 0;

	// ブロック
	std::vector<Block*> blocks_{};

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時更新処理
	void OnCollision();

	// 行列更新
	void MatUpdate();

private:
	// 移動処理
	void Move();

	// 回転処理
	void Rotate();
#pragma endregion

#pragma region ゲッター関数
public:
	// プレイヤーの座用を取得
	const Vector2& GetPosition() { return sprite_->GetPosition(); }
#pragma endregion
};