#pragma once
#include "BaseScene.h"
#include "Key.h"

#include "Camera.h"
#include "Sprite.h"

#include <memory>

class GameScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Key* key_ = nullptr;// キーボード入力

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;
#pragma endregion

#pragma region メンバ関数
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
	// 衝突時処理
	void OnCollision();

	// オブジェクト更新処理
	void ObjUpdate();
#pragma endregion
};