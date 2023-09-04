#pragma once
#include "Object3D.h"
#include "SphereCollider.h"
#include "RayCollider.h"
#include "Key.h"
#include "CollisionManager.h"

#include <memory>

class TestPlayer
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;				// キーボード入力
	CollisionManager* colMgr_ = nullptr;// コリジョンマネージャー

	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;
	std::unique_ptr<Object3D> rayObj_ = nullptr;

	// 球コライダー
	std::unique_ptr<RayCollider> collider_ = nullptr;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	TestPlayer();

	// デストラクタ
	~TestPlayer();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時
	void OnCollision();

	// オブジェクト更新処理
	void ObjUpdate();
#pragma endregion
};