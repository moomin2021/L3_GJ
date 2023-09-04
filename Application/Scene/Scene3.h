#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Mouse.h"
#include "PointCollider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Sprite.h"
#include "CollisionManager2D.h"

#include <memory>

class Scene3 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;
	Mouse* mouse_ = nullptr;

	CollisionManager2D* colMgr2D_ = nullptr;

	std::unique_ptr<PointCollider> pCol_ = nullptr;
	std::unique_ptr<BoxCollider> boxCol_ = nullptr;
	std::unique_ptr<CircleCollider> cirCol_ = nullptr;

	std::unique_ptr<Sprite> sBox_ = nullptr;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene3();

	// デストラクタ
	~Scene3();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// 衝突判定後処理
	void OnCollision();

	// オブジェクト更新
	void ObjUpdate();
};

