#pragma once
#include "BaseScene.h"
#include "Pad.h"
#include "Key.h"
#include "Camera.h"
#include "Sprite.h"
#include "Fade.h"

#include <memory>

class ResultScene : public BaseScene
{
#pragma region メンバ変数
private:
	// インスタンス
	Pad* pad_ = nullptr;
	Key* key_ = nullptr;

	// フェード
	std::unique_ptr<Fade> fade_ = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sBackGround_ = nullptr;
	std::unique_ptr<Sprite> sFrame_ = nullptr;
	std::unique_ptr<Sprite> sResultLogo_ = nullptr;
	std::unique_ptr<Sprite> sTimeText_ = nullptr;
	std::unique_ptr<Sprite> sRetryText_ = nullptr;
	std::unique_ptr<Sprite> sTitleText_ = nullptr;
	std::unique_ptr<Sprite> sRankPiece_ = nullptr;
	std::unique_ptr<Sprite> sRankText_ = nullptr;
	std::unique_ptr<Sprite> sNextRankText_ = nullptr;
	std::unique_ptr<Sprite> sResultSelectFrame_ = nullptr;

	// 画像ハンドル
	uint16_t hBackGround_ = 0;
	uint16_t hFrame_ = 0;
	uint16_t hResultLogo_ = 0;
	uint16_t hTimeText_ = 0;
	uint16_t hRetryText_ = 0;
	uint16_t hTitleText_ = 0;
	uint16_t hRankPiece_ = 0;
	uint16_t hRankText_ = 0;
	uint16_t hNextRankText_ = 0;
	uint16_t hResultSelectFrame_ = 0;

	// 選択時座標
	std::vector<Vector2> selectPos_ = {
		{ 464.0f, 624.0f },
		{ 464.0f, 752.0f },
	};

	// 選択数字
	INT16 selectNum_ = 0;

	// LスティックY座標保存
	float oldLStickY = 0.0f;

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	ResultScene();

	// デストラクタ
	~ResultScene();

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

