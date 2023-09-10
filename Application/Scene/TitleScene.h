#pragma once
#include "BaseScene.h"
#include "Pad.h"
#include "Camera.h"
#include "Sprite.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Pad* pad_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sBackGround_ = nullptr;

	// 画像ハンドル
	uint16_t hBackGround_ = 0;

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

private:
	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();
#pragma endregion
};

