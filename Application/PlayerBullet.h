#pragma once
#include "Sprite.h"
#include "Boss.h"
#include "CollisionManager2D.h"
#include "BoxCollider.h"

#include<memory>

class PlayerBullet {
#pragma region メンバ変数
private:
	static Vector2	sBulletSize_;	// 弾のサイズ
	static uint16_t	sHandle_;		// 弾の画像ハンドル
	static Boss*	sBoss_;			// ボスのポインタ
	static CollisionManager2D* sColMgr2D_;// 衝突マネージャー2D

	// 座標
	Vector2 position_ = { 0.0f, 0.0f };

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// コライダー
	std::unique_ptr<BoxCollider> collider_ = nullptr;

	// 移動方向
	Vector2 velocity_ = { 0.0f, 0.0f };

	// 生存フラグ
	bool isAlive_ = true;
#pragma endregion

#pragma region メンバ関数
public:
	// デストラクタ
	~PlayerBullet();

	// 初期化処理
	void Initialize(const Vector2& pos, const Vector2& velocity);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision(uint16_t damage);

	// 行列更新
	void MatUpdate();
#pragma endregion

#pragma region セッター関数
	// テクスチャハンドルを設定
	static void SetHandle(uint16_t handle) { sHandle_ = handle; }

	// ボスを設定
	static void SetBoss(Boss* boss) { sBoss_ = boss; }

	// 衝突マネージャー2Dを設定
	static void SetColMgr2D(CollisionManager2D* colMgr2D) { sColMgr2D_ = colMgr2D; }
#pragma endregion

#pragma region ゲッター関数
	// 生存フラグを取得
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};