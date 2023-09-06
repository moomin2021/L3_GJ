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
#include "TestPlayer.h"
#include "TestEnemy.h"
#include"Player.h"
#include"Piece.h"

#include <memory>

class Scene1 : public BaseScene
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

	// BGMキー
	uint16_t bgmKey_ = 0;

	// プレイヤー
	std::unique_ptr<TestPlayer> player_ = nullptr;
	std::unique_ptr<Player> player = nullptr;

	// エネミー
	std::unique_ptr<TestEnemy> enemy_ = nullptr;

	//ピース配列
	std::vector < std::unique_ptr<Piece>> pieces;

	// --メンバ関数-- //
public:
	// コンストラクタ
	Scene1();

	// デストラクタ
	~Scene1();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// オブジェクト更新処理
	void ObjUpdate();

	// 衝突時処理
	void OnCollision();
};

