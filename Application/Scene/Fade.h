#pragma once
#include "Sprite.h"
#include "SceneManager.h"

#include <memory>

class Fade
{
#pragma region メンバ変数
private:
	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// 画像ハンドル
	uint16_t handle_ = 0;

	// フェードインアウトフラグ
	static bool isFade_;
	static bool isFadeIn_;
	static bool isFadeOut_;

	float fadeInTime_ = 1.0f;
	float fadeOutTime_ = 1.0f;
	uint64_t startTime_ = 0;

	static SCENE nextScene_;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 行列更新
	void MatUpdate();

	// シーン切り替え
	void ChangeScene(SCENE sceneNum);
#pragma endregion
};