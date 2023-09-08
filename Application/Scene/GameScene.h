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
#include "Boss.h"

#include <memory>

class GameScene : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player = nullptr;

	// ボス
	std::unique_ptr<Boss> boss_ = nullptr;

	//ピース配列
	std::vector < std::unique_ptr<Piece>> pieces;

	//背景テクスチャとスプライト
	uint16_t backGroundTexture = 0;
	std::unique_ptr<Sprite> backGroundSprite = nullptr;

	// --メンバ関数-- //
public:
	// コンストラクタ
	GameScene();

	// デストラクタ
	~GameScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// 行列更新処理
	void MatUpdate();

	// 衝突時処理
	void OnCollision();
};

