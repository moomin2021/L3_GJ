#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Camera.h"
#include "Sprite.h"
#include "BoxCollider.h"

#include <memory>

class Scene2 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_[2];

	// コライダー
	std::unique_ptr<BoxCollider> collider_[2];

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene2();

	// デストラクタ
	~Scene2();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// 衝突時処理
	void OnCollision();

	// 行列更新
	void MatUpdate();
};

