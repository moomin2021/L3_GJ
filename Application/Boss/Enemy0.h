#pragma once
#include "Sprite.h"

#include <memory>

class Enemy0
{
#pragma region メンバ変数
private:
	// 画像ハンドル
	static uint16_t hBack_;
	static uint16_t hFront_;

	// スプライト
	std::unique_ptr<Sprite> sBack_ = nullptr;// 裏面
	std::unique_ptr<Sprite> sFront_ = nullptr;// 表面

	// 生存フラグ
	bool isAlive_ = true;

	// 座標
	Vector2 position_ = { 0.0f, 0.0f };

	// 回転
	float rotate_ = 0.0f;

	// 速度
	float speed_ = 10.0f;

	// 回転速度
	float rotateSpd_ = 3.0f;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Enemy0();

	// デストラクタ
	~Enemy0();

	// 初期化処理
	void Initialize(Vector2 pos);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion

#pragma region セッター関数
	// 裏面ハンドルを設定
	static void SetBackHandle(uint16_t handle) { hBack_ = handle; }

	// 表面ハンドルを設定
	static void SetFrontHandle(uint16_t handle) { hFront_ = handle; }
#pragma endregion

#pragma region ゲッター関数
	// 生存フラグを取得
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};