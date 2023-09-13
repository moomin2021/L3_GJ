#pragma once
#include "BaseScene.h"// -> 各シーンのベース
#include "Key.h"// -> キーボード入力
#include "PostEffect.h"

#include <memory>

// シーン列挙型
enum SCENE {
	TITLE,
	GAME,
	RESULT,
};

enum PostEffectType {
	NORMAL,
	BLUR,
	BLOOM
};

class SceneManager {
	// --メンバ変数-- //
public:

private:
	// キーボード入力クラス
	Key* key_;

	// 現在のシーン
	std::unique_ptr<BaseScene> nowScene_;

	// ポストエフェクト
	std::unique_ptr<PostEffect> gaussianPostEffect_;
	std::unique_ptr<PostEffect> highLumiPostEffect_;
	std::unique_ptr<PostEffect> bloomPostEffect_;

	// ポストエフェクトの種類
	PostEffectType postEffectType_;

	bool isChangeSceme_ = false;

	SCENE nextScene_ = TITLE;

	bool isEnd_ = false;

	// --メンバ関数-- //
public:
	// インスタンス取得
	static SceneManager* GetInstance();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	void Post();

	// シーン切り替え
	void ChangeScene(SCENE changeSceneNum);

	bool GetIsEnd() { return isEnd_; }

	void SetIsEnd(bool frag) { isEnd_ = frag; }
private:
	// コンストラクタ
	SceneManager();

	// デストラクタ
	~SceneManager();

	// --禁止-- //
public:
	// コピーコンストラクタ
	SceneManager(const SceneManager& instance) = delete;

	// 代入演算子
	SceneManager& operator=(const SceneManager& instance) = delete;
};