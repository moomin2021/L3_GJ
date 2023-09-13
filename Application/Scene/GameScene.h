#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Camera.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Object3D.h"
#include "RayCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"
#include"Player.h"
#include"Piece.h"
#include "Boss.h"
#include "UIManager.h"
#include "ParticleEmitter2D.h"
#include"PieceManager.h"
#include "Fade.h"
#include "Sound.h"

#include <memory>

class GameScene : public BaseScene
{
	// --メンバ変数-- //
private:
	// キーボード入力
	Key* key_ = nullptr;
	Sound* sound_ = nullptr;

	// サウンドハンドル
	uint16_t gameBGM_ = 0;

	// フェード
	std::unique_ptr<Fade> fade_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// プレイヤー
	std::unique_ptr<Player> player = nullptr;

	// ボス
	std::unique_ptr<Boss> boss_ = nullptr;

	//ピース管理クラス
	std::unique_ptr<PieceManager> psManager = nullptr;

	//背景テクスチャとスプライト
	uint16_t backGroundTexture = 0;
	std::unique_ptr<Sprite> backGroundSprite = nullptr;

	// UIマネージャー
	std::unique_ptr<UIManager> uiMgr_ = nullptr;

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

