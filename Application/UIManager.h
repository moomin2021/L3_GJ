#pragma once
#include "Sprite.h"

#include <memory>

class UIManager
{
#pragma region メンバ変数
private:
	// スプライト
	std::unique_ptr<Sprite> sFrame_ = nullptr;
	std::unique_ptr<Sprite> sPlayerHpIn_ = nullptr;

	// 画像ハンドル
	uint16_t hFrame_ = 0;
	uint16_t hPlayerHpIn_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	UIManager();

	// デストラクタ
	~UIManager();

	// 初期化処理
	void Initialize();

	// 行列更新処理
	void MatUpdate();

	// 描画処理
	void Draw();
#pragma endregion
};