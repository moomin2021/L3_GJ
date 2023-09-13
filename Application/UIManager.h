#pragma once
#include "Sprite.h"

#include <memory>

class Boss;

class UIManager
{
#pragma region メンバ変数
private:
	// ボス
	static Boss* sBoss_;

	// スプライト
	std::unique_ptr<Sprite> sTitleFrame_ = nullptr;
	std::unique_ptr<Sprite> sExp_ = nullptr;
	std::unique_ptr<Sprite> sExpIn_ = nullptr;
	std::unique_ptr<Sprite> sExpText_ = nullptr;
	std::unique_ptr<Sprite> sHpPlayer_ = nullptr;
	std::unique_ptr<Sprite> sHpPlayerIn_ = nullptr;
	std::unique_ptr<Sprite> sHpTextPlayer_ = nullptr;
	std::unique_ptr<Sprite> sHpBoss_ = nullptr;
	std::unique_ptr<Sprite> sHpTextBoss_ = nullptr;
	std::unique_ptr<Sprite> sNextText_ = nullptr;
	std::vector<std::unique_ptr<Sprite>> sTimes_ = {};
	std::unique_ptr<Sprite> sColon_ = nullptr;

	// 画像ハンドル
	uint16_t hTitleFrame_ = 0;
	uint16_t hExp_ = 0;
	uint16_t hExpIn_ = 0;
	uint16_t hExpText_ = 0;
	uint16_t hHpPlayer_ = 0;
	uint16_t hHpPlayerIn_ = 0;
	uint16_t hHpTextPlayer_ = 0;
	uint16_t hHpBoss_ = 0;
	uint16_t hHpTextBoss_ = 0;
	uint16_t hNextText_ = 0;
	std::vector<uint16_t> hNumbers_ = {};
	uint16_t hColon_ = 0;

	// ゲーム経過時間
	uint64_t startGameTime_ = 0;
	std::vector<uint16_t> gameTimes_ = {};
	std::vector<uint16_t> clearTime_ = {};

	// クリアフラグ
	bool isClear_ = false;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	UIManager();

	// デストラクタ
	~UIManager();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 行列更新処理
	void MatUpdate();

	// 描画処理
	void Draw();
#pragma endregion

#pragma region セッター関数
	// ボスを設定
	static void SetBoss(Boss* boss) { sBoss_ = boss; }
#pragma endregion
};