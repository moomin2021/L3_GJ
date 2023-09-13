#pragma once
#include "BaseScene.h"
#include "Pad.h"
#include "Camera.h"
#include "Sprite.h"
#include "Fade.h"
#include "Key.h"
#include "Sound.h"

#include <memory>

class TitleScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Pad* pad_ = nullptr;
	Key* key_ = nullptr;
	Sound* sound_ = nullptr;

	// フェード
	std::unique_ptr<Fade> fade_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sBackGround_ = nullptr;
	std::unique_ptr<Sprite> sTitleFrame_ = nullptr;
	std::unique_ptr<Sprite> sTitleLogo_ = nullptr;
	std::unique_ptr<Sprite> sStartText_ = nullptr;
	std::unique_ptr<Sprite> sExitText_ = nullptr;
	std::unique_ptr<Sprite> sTitleSelectFrame_ = nullptr;;

	// 画像ハンドル
	uint16_t hBackGround_ = 0;
	uint16_t hTitleFrame_ = 0;
	uint16_t hTitleLogo_ = 0;
	uint16_t hStartText_ = 0;
	uint16_t hExitText_ = 0;
	uint16_t hTitleSelectFrame_ = 0;

	// サウンドハンドル
	uint16_t sTitleBGM_ = 0;
	uint16_t selectSE_ = 0;
	uint16_t selectMoveSE_ = 0;

	// 選択時座標
	std::vector<Vector2> selectPos_ = {
		{ 628.0f, 740.0f },
		{ 1228.0f, 740.0f },
	};

	// 選択数字
	INT16 selectNum_ = 0;

	// LスティックX座標保存
	float oldLStickX= 0.0f;

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

