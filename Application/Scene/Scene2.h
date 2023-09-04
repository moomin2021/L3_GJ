#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Camera.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Object3D.h"
#include "ParticleEmitter.h"
#include "RayCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"

#include <memory>

class Scene2 : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// ライトグループ
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	// 平行光源
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_ = nullptr;

	// オブジェクト
	std::unique_ptr<Object3D> object_ = nullptr;

	// パーティクルエミッター
	std::vector<std::unique_ptr<ParticleEmitter>> particleEmitters_;

	// レイコライダー
	std::unique_ptr<RayCollider> rayCol_ = nullptr;

	// メッシュコライダー
	std::unique_ptr<MeshCollider> meshCol_ = nullptr;

	// 球コライダー
	std::unique_ptr<SphereCollider> sphereCol_ = nullptr;

	// パーティクル用画像ハンドル
	uint16_t particlehandle_ = 0;

	// BGMキー
	uint16_t bgmKey_ = 0;

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
};

