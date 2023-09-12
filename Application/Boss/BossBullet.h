#pragma once
#include "Sprite.h"
#include "BoxCollider.h"
#include "CollisionManager2D.h"

#include <memory>

class BossBullet
{
#pragma region メンバ変数
private:
	// 画像ハンドル
	static uint16_t sHandle_;

	// 弾の速度
	static float sSpeed_;

	// 衝突マネージャー2D
	static CollisionManager2D* sColMgr2D_;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// コライダー
	std::unique_ptr<BoxCollider> collider_ = nullptr;

	// 座標
	Vector2 position_ = { 0.0f, 0.0f };

	// 生存フラグ
	bool isAlive_ = true;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	BossBullet();

	// デストラクタ
	~BossBullet();

	// 初期化処理
	void Initialize(Vector2 pos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新
	void MatUpdate();
#pragma endregion

#pragma region セッター関数
	// ハンドルを設定
	static void SetHandle(uint16_t handle) { sHandle_ = handle; }

	// 速度を設定
	static void SetSpeed(float speed) { sSpeed_ = speed; }

	// 衝突マネージャーを設定
	static void SetColMgr2D(CollisionManager2D* colMgr2D) { sColMgr2D_ = colMgr2D; }
#pragma endregion

#pragma region ゲッター関数
	// 生存フラグを取得
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};