#pragma once
#include "Sprite.h"
#include "BossBullet.h"

//#include <memory>
#include <vector>

class Boss
{
#pragma region 列挙型
private:
	enum State {
		WAIT,		// 待機
		MOVE_SHOT,	// 移動撃ち
		SUMMON,		// 召喚
		BOOMERANG,	// ブーメラン
	};
#pragma endregion

#pragma region メンバ変数
private:
	// スプライト
	std::unique_ptr<Sprite> sBossBack0_ = nullptr;	// ボス裏面0
	std::unique_ptr<Sprite> sBossBack1_ = nullptr;	// ボス裏面1
	std::unique_ptr<Sprite> sBossFront_ = nullptr;	// ボス前面

	// 画像ハンドル
	uint16_t hBossBack_ = 0;
	uint16_t hBossFront_ = 0;

	// 状態
	State state_ = MOVE_SHOT;

	// ボスの座標
	Vector2 position_ = { 1500.0f, 540.0f };

	// ボス裏面の角度
	float rotateBossBack0_ = 0.0f;
	float rotateBossBack1_ = 45.0f;

	// ボス裏面の回転速度
	float rotateSpd_ = 3.0f;

	// ボスの移動速度
	float moveSpd_ = 0.05f;

	// 弾
	std::vector<std::unique_ptr<BossBullet>> bullets_;

	// 弾を撃つ間隔[s]
	float shotInterval_ = 1.0f;

	// 弾を撃った時間
	uint64_t shotTime_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Boss();

	// デストラクタ
	~Boss();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新
	void MatUpdate();

private:
	// 状態別更新処理
	static void (Boss::* stateTable[]) ();

	// 状態別行動
	void Wait();		// 待機
	void MoveShot();	// 移動撃ち
	void Summon();		// 召喚
	void Boomerang();	// ブーメラン
#pragma endregion
};