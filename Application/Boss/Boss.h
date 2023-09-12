#pragma once
#include "Sprite.h"
#include "BossBullet.h"
#include "Enemy0.h"
#include "ParticleEmitter2D.h"
#include "Key.h"
#include "CircleCollider.h"
#include "CollisionManager2D.h"

#include <memory>
#include <string>
#include <vector>

class Player;

class Boss
{
#pragma region 列挙型
private:
	enum State {
		WAIT,			// 待機
		PRE_MOVE_SHOT,	// 移動撃ち前処理
		MOVE_SHOT,		// 移動撃ち
		POST_MOVE_SHOT,	// 移動撃ち後処理
		PRE_SUMMON,		// 召喚前準備
		SUMMON,			// 召喚
		POST_SUMMON,	// 召喚あと処理
		PRE_BOOMERANG,	// ブーメラン前処理
		BOOMERANG,		// ブーメラン
		POST_BOOMERANG,	// ブーメラン後処理
	};
#pragma endregion

#pragma region メンバ変数
private:
	std::vector<std::string> stateText_ = {
		"WAIT",
		"PRE_MOVE_SHOT", "MOVE_SHOT", "POST_MOVE_SHOT",
		"PRE_SUMMON", "SUMMON", "POST_SUMMON",
		"PRE_BOOMERANG", "BOOMERANG", "POST_BOOMERANG",
	};

	// インスタンス
	Key* key_ = nullptr;
	CollisionManager2D* colMgr2D_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sBossBack0_ = nullptr;	// ボス裏面0
	std::unique_ptr<Sprite> sBossBack1_ = nullptr;	// ボス裏面1
	std::unique_ptr<Sprite> sBossFront_ = nullptr;	// ボス前面
	std::unique_ptr<Sprite> sHpBossIn_ = nullptr;// ボスのHPゲージ
	std::unique_ptr<Sprite> sKakeru_ = nullptr;// HPゲージの個数表示

	// 画像ハンドル
	uint16_t hBossBack_ = 0;
	uint16_t hBossFront_ = 0;
	uint16_t hParticle_ = 0;
	uint16_t hHpBossIn_ = 0;

	// コライダー
	std::unique_ptr<CircleCollider> collider_ = nullptr;

	// 状態
	State state_ = WAIT;

	// HP
	size_t gaugeNum_ = 4;
	std::vector<uint16_t> hp_ = {};
	uint16_t oneGaugeValue_ = 200;

	// 生存フラグ
	bool isAlive_ = true;

	// 座標
	Vector2 position_ = { 1500.0f, 540.0f };// ボスの座標
	Vector2 basicPos_ = { 1500.0f, 540.0f };// 基本座標
	Vector2 backPos0_ = { 1500.0f, 540.0f };// 裏面座標0
	Vector2 backPos1_ = { 1500.0f, 540.0f };// 裏面座標1

	// sin関数での移動数値
	float sinMove_ = 0.0f;	// sin関数に渡す数値
	float sinSpd_ = 0.05f;	// sin値の加算速度

	// パーティクルエミッター
	std::unique_ptr<ParticleEmitter2D> emitterBack0_ = nullptr;
	std::unique_ptr<ParticleEmitter2D> emitterBack1_ = nullptr;

	// ボス裏面の角度
	float rotateBossBack0_ = 0.0f;
	float rotateBossBack1_ = 45.0f;
	float rotateBossFront_ = 0.0f;

	// ボス裏面の回転速度
	float basicSpd_ = 3.0f;
	float moveShotRotaSpd_ = 30.0f;
	float summonRotaSpd_ = 30.0f;
	float boomerangRotaSpd_ = 30.0f;

	// 敵
	std::vector<std::unique_ptr<Enemy0>> enemys_;
	float summonInterval_ = 1.0f;	// 召喚時間
	uint64_t summonTime_ = 0;		// 召喚した時間

	// 弾関連
	std::vector<std::unique_ptr<BossBullet>> bullets_;	// 弾
	float shotInterval_ = 1.0f;							// 弾を撃つ間隔[s]
	uint64_t shotTime_ = 0;								// 弾を撃った時間

	// 行動遷移関連
	Vector2 beforePos_ = { 0.0f, 0.0f };	// 行動遷移前座標
	Vector2 beforeBackPos0_ = { 0.0f, 0.0f };// 裏面0行動前座標
	Vector2 beforeBackPos1_ = { 0.0f, 0.0f };// 裏面0行動前座標
	uint64_t actionStartTime_= 0;	// 行動開始時間
	float time2PreMoveShot_		= 3.0f;	// 移動撃ち前時間
	float time2MoveShot_		= 10.0f;// 移動撃ち時間
	float time2PostMoveShot_	= 3.0f;	// 移動撃ち後時間
	float time2PreSummon_		= 3.0f;	// 召喚前時間
	float time2Summon_			= 10.0f;// 召喚する時間
	float time2PostSummon_		= 3.0f;	// 召喚後時間
	float time2PreBoomerang_	= 3.0f;	// 召喚前時間
	float time2Boomerang_		= 10.0f;// 召喚する時間
	float time2PostBoomerang_	= 3.0f;	// 召喚後時間

	bool isBoomerang_ = false;

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

	// UI描画
	void UIDraw();

	// 衝突時処理
	void OnCollision();

	// 行列更新
	void MatUpdate();

	// ボスのHP減少
	void SubHP(uint16_t value);

private:
	// 状態別更新処理
	static void (Boss::* stateTable[]) ();

	// 状態別行動
	void Wait();			// 待機
	void PreMoveShot();		// 移動撃ち前処理
	void MoveShot();		// 移動撃ち
	void PostMoveShot();	// 移動撃ち後処理
	void PreSummon();		// 召喚前準備
	void Summon();			// 召喚
	void PostSummon();		// 召喚後処理
	void PreBoomerang();	// ブーメラン前処理
	void Boomerang();		// ブーメラン
	void PostBoomerang();	// ブーメラン後処理

	void BossBackRotate(float rotate);// ボスの裏面回転

	// ボスのHP更新
	void HPUpdate();

	// ImGuiを使う
	void DebugImGui();
#pragma endregion
};