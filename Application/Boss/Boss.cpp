#include "Boss.h"
#include "Texture.h"
#include "Util.h"
#include "Easing.h"
#include "PipelineManager.h"

#include <imgui_impl_DX12.h>

Boss::Boss() {}

Boss::~Boss() {
}

void Boss::Initialize()
{
#pragma region インスタンス取得
	key_ = Key::GetInstance();
#pragma endregion

#pragma region スプライト
	// ボス裏面0
	sBossBack0_ = std::make_unique<Sprite>();
	sBossBack0_->SetPosition(backPos0_);
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack0_->SetSize({ 160.0f, 160.0f });
	sBossBack0_->SetAnchorPoint({ 0.5f, 0.5f });

	// ボス裏面1
	sBossBack1_ = std::make_unique<Sprite>();
	sBossBack1_->SetPosition(backPos1_);
	sBossBack1_->SetRotation(rotateBossBack1_);
	sBossBack1_->SetSize({ 160.0f, 160.0f });
	sBossBack1_->SetAnchorPoint({ 0.5f, 0.5f });

	// ボス裏面2
	sBossFront_ = std::make_unique<Sprite>();
	sBossFront_->SetPosition(position_);
	sBossFront_->SetSize({ 128.0f, 128.0f });
	sBossFront_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region 画像ハンドル
	hBossBack_ = LoadTexture("Resources/boss_back.png");
	hBossFront_ = LoadTexture("Resources/boss_Front.png");
	hParticle_ = LoadTexture("Resources/particle_enemy.png");
#pragma endregion

#pragma region BossBullet
	// 画像ハンドルを設定
	BossBullet::SetHandle(LoadTexture("Resources/bullet_enemy.png"));

	// 速度を設定
	BossBullet::SetSpeed(10.0f);
#pragma endregion

#pragma region Enemy0
	Enemy0::SetBackHandle(LoadTexture("Resources/enemy_back.png"));
	Enemy0::SetFrontHandle(LoadTexture("Resources/enemy_front.png"));
#pragma endregion

#pragma region パーティクルエミッター
	emitterBack0_ = std::make_unique<ParticleEmitter2D>(120);
	emitterBack1_ = std::make_unique<ParticleEmitter2D>(120);
#pragma endregion
}

void Boss::Update()
{
	// 状態別更新処理
	(this->*stateTable[state_])();

#pragma region 弾の後処理
	// 弾の生存フラグが[OFF]なら消す
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		// 弾の更新
		(*it)->Update();

		// 弾の生存フラグが[OFF]になったら弾を削除
		if ((*it)->GetIsAlive() == false) it = bullets_.erase(it);
		else ++it;
	}
#pragma endregion

#pragma region 雑魚敵の後処理
	// 雑魚敵の生存フラグが[OFF]なら消す
	for (auto it = enemys_.begin(); it != enemys_.end();) {
		// 雑魚敵の更新
		(*it)->Update();

		// 雑魚敵の生存フラグが[OFF]になったら弾を削除
		if ((*it)->GetIsAlive() == false) it = enemys_.erase(it);
		else ++it;
	}
#pragma endregion

	// デバック用
	DebugImGui();
}

void Boss::Draw()
{
	PipelineManager::PreDraw("Sprite");

	sBossBack0_->Draw(hBossBack_);// ボス裏面0
	sBossBack1_->Draw(hBossBack_);// ボス裏面1
	sBossFront_->Draw(hBossFront_);// ボス表面

	// 弾
	for (auto& it : bullets_) {
		it->Draw();
	}

	// 雑魚敵
	for (auto& it : enemys_) {
		it->Draw();
	}

	PipelineManager::PreDraw("Particle2D", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	emitterBack0_->Draw(hParticle_);
	emitterBack1_->Draw(hParticle_);
}

void Boss::OnCollision()
{
	// 弾
	for (auto& it : bullets_) {
		it->OnCollision();
	}

	// 雑魚敵
	for (auto& it : enemys_) {
		it->OnCollision();
	}
}

void Boss::MatUpdate()
{
	sBossBack0_->MatUpdate();// ボス裏面0
	sBossBack1_->MatUpdate();// ボス裏面1
	sBossFront_->MatUpdate();// ボス表面

	// 弾
	for (auto& it : bullets_) {
		it->MatUpdate();
	}

	// 雑魚敵
	for (auto& it : enemys_) {
		it->MatUpdate();
	}

#pragma region パーティクルエミッター
	emitterBack0_->Update();
	emitterBack1_->Update();
#pragma endregion
}

void (Boss::* Boss::stateTable[]) () = {
	&Boss::Wait,			// 待機
	&Boss::PreMoveShot,		// 移動撃ち前処理
	&Boss::MoveShot,		// 移動撃ち
	&Boss::PostMoveShot,	// 移動撃ち後処理
	&Boss::PreSummon,		// 召喚前準備
	&Boss::Summon,			// 召喚
	&Boss::PostSummon,		// 召喚後処理
	&Boss::PreBoomerang,	// ブーメラン前処理
	&Boss::Boomerang,		// ブーメラン
	&Boss::PostBoomerang,	// ブーメラン後処理
};

void Boss::Wait()
{
	// ボスの裏面回転
	BossBackRotate(basicSpd_);
}

void Boss::PreMoveShot()
{
#pragma region 遷移前座標から基本座標まで移動
	// 行動開始からの経過時間
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// 経過時間の割合で移動
	float rate = Util::Clamp(elapsedTime / time2PreMoveShot_, 1.0f, 0.0f);

	// 経過時間が指定時間以上ならStateをSummonにする
	if (elapsedTime >= time2PreMoveShot_) {
		state_ = MOVE_SHOT;
		sinMove_ = 0.0f;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region 裏面回転
	// ボスの裏面回転
	BossBackRotate(Easing::Quint::easeOut(basicSpd_, moveShotRotaSpd_, rate));
#pragma endregion
}

void Boss::MoveShot()
{
	// ボスの裏面回転
	BossBackRotate(moveShotRotaSpd_);

#pragma region sin関数で移動処理
	sinMove_ += sinSpd_;
	position_.y = (sinf(sinMove_) * 300.0f) + 540.0f;
	backPos0_ = position_;
	backPos1_ = position_;
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
	sBossFront_->SetPosition(position_);
#pragma endregion

#pragma region 弾を撃つ
	// 弾を撃ってからの経過時間[s]
	float elapsedTime = (Util::GetTimrMSec() - shotTime_) / 1000.0f;

	// 経過時間がインターバル以上なら弾を撃つ
	if (elapsedTime >= shotInterval_) {
		// 撃った時間を保存
		shotTime_ = Util::GetTimrMSec();

		// 弾を生成
		bullets_.emplace_back(std::make_unique<BossBullet>());
		bullets_.back()->Initialize(sBossFront_->GetPosition());
	}
#pragma endregion

#pragma region 移動撃ち時間管理
	// 移動撃ちを開始してから指定の時間が経っていたら召喚を終了
	elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	if (elapsedTime >= time2MoveShot_) {
		state_ = POST_MOVE_SHOT;
		actionStartTime_ = Util::GetTimrMSec();
		beforePos_ = position_;
	}
#pragma endregion
}

void Boss::PostMoveShot()
{
#pragma region 遷移前座標から基本座標まで移動
	// 行動開始からの経過時間
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// 経過時間の割合で移動
	float rate = Util::Clamp(elapsedTime / time2PostMoveShot_, 1.0f, 0.0f);
	position_.x = Easing::Quint::easeOut(beforePos_.x, basicPos_.x, rate);
	position_.y = Easing::Quint::easeOut(beforePos_.y, basicPos_.y, rate);
	backPos0_ = position_;
	backPos1_ = position_;

	// 経過時間が指定時間以上ならStateをWAITにする
	if (elapsedTime >= time2PostMoveShot_) {
		state_ = WAIT;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region 裏面回転
	// ボスの裏面回転
	BossBackRotate(Easing::Quint::easeOut(moveShotRotaSpd_, basicSpd_, rate));
#pragma endregion

#pragma region スプライトデータ更新
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
	sBossFront_->SetPosition(position_);
#pragma endregion
}

void Boss::PreSummon()
{
#pragma region 遷移前座標から基本座標まで移動
	// 行動開始からの経過時間
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// 経過時間の割合で移動
	float rate = Util::Clamp(elapsedTime / time2PreSummon_, 1.0f, 0.0f);

	// 経過時間が指定時間以上ならStateをSummonにする
	if (elapsedTime >= time2PreSummon_) {
		state_ = SUMMON;
		sinMove_ = 0.0f;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region 裏面回転
	// ボスの裏面回転
	BossBackRotate(Easing::Quint::easeOut(basicSpd_, summonRotaSpd_, rate));
#pragma endregion

#pragma region スプライトデータ更新
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
	sBossFront_->SetPosition(position_);
#pragma endregion
}

void Boss::Summon()
{
	// ボスの裏面回転
	BossBackRotate(summonRotaSpd_);

#pragma region パーティクル
	float rnd0X = Util::GetRandomFloat(0.0f, 1.0f) * 2.0f - 1.0f;
	float rnd0Y = Util::GetRandomFloat(0.0f, 1.0f) * 2.0f - 1.0f;
	emitterBack0_->Add(60, { rnd0X * 10.0f, rnd0Y * 10.0f }, { 3.0f * rnd0X, 3.0f * rnd0Y }, { 0.05f * -rnd0X, 0.05f * rnd0Y }, 32.0f, 0.0f);
	emitterBack0_->SetPosition(backPos0_);

	float rnd1X = Util::GetRandomFloat(0.0f, 1.0f) * 2.0f - 1.0f;
	float rnd1Y = Util::GetRandomFloat(0.0f, 1.0f) * 2.0f - 1.0f;
	emitterBack1_->Add(60, { rnd1X * 10.0f, rnd1Y * 10.0f }, { 3.0f * rnd1X, 3.0f * rnd1Y }, { 0.05f * -rnd1X, 0.05f * rnd1Y }, 32.0f, 0.0f);
	emitterBack1_->SetPosition(backPos1_);
#pragma endregion

#pragma region sin関数で移動処理
	sinMove_ += sinSpd_;
	backPos0_.y = (sinf(sinMove_) * 300.0f) + 540.0f;;
	backPos1_.y = (-sinf(sinMove_) * 300.0f) + 540.0f;;
#pragma endregion

#pragma region 雑魚敵召喚
	// 弾を撃ってからの経過時間[s]
	float elapsedTime = (Util::GetTimrMSec() - summonTime_) / 1000.0f;

	// 経過時間がインターバル以上なら召喚する
	if (elapsedTime >= summonInterval_) {
		// 召喚した時間を保存
		summonTime_ = Util::GetTimrMSec();

		// 敵を生成
		enemys_.emplace_back(std::make_unique<Enemy0>());
		enemys_.back()->Initialize(backPos0_);
		enemys_.emplace_back(std::make_unique<Enemy0>());
		enemys_.back()->Initialize(backPos1_);
	}
#pragma endregion

#pragma region 召喚時間管理
	// 召喚してから指定の時間が経っていたら召喚を終了
	elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	if (elapsedTime >= time2Summon_) {
		state_ = POST_SUMMON;
		actionStartTime_ = Util::GetTimrMSec();
		beforeBackPos0_ = backPos0_;
		beforeBackPos1_ = backPos1_;
	}
#pragma endregion

#pragma region スプライトデータ更新
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
#pragma endregion
}

void Boss::PostSummon()
{

#pragma region 遷移前座標から基本座標まで移動
	// 行動開始からの経過時間
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// 経過時間の割合で移動
	float rate = Util::Clamp(elapsedTime / time2PostSummon_, 1.0f, 0.0f);
	backPos0_.x = Easing::Quint::easeOut(beforeBackPos0_.x, basicPos_.x, rate);
	backPos0_.y = Easing::Quint::easeOut(beforeBackPos0_.y, basicPos_.y, rate);
	backPos1_.x = Easing::Quint::easeOut(beforeBackPos1_.x, basicPos_.x, rate);
	backPos1_.y = Easing::Quint::easeOut(beforeBackPos1_.y, basicPos_.y, rate);

	// 経過時間が指定時間以上ならStateをSummonにする
	if (elapsedTime >= time2PostSummon_) {
		state_ = WAIT;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region 裏面回転
	// ボスの裏面回転
	BossBackRotate(Easing::Quint::easeOut(summonRotaSpd_, basicSpd_, rate));
#pragma endregion

#pragma region スプライトデータ更新
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
#pragma endregion
}

void Boss::PreBoomerang()
{
#pragma region 遷移前座標から基本座標まで移動
	// 行動開始からの経過時間
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// 経過時間の割合で移動
	float rate = Util::Clamp(elapsedTime / time2PreBoomerang_, 1.0f, 0.0f);

	// 経過時間が指定時間以上ならStateをBoomerangにする
	if (elapsedTime >= time2PreBoomerang_) {
		state_ = BOOMERANG;
		actionStartTime_ = Util::GetTimrMSec();
	}

	// 経過時間の割合で移動
	backPos0_.x = Easing::Quint::easeOut(basicPos_.x, 1500.0f, rate);
	backPos0_.y = Easing::Quint::easeOut(basicPos_.y, 840.0f, rate);
	backPos1_.x = Easing::Quint::easeOut(basicPos_.x, 1500.0f, rate);
	backPos1_.y = Easing::Quint::easeOut(basicPos_.y, 840.0f, rate);
#pragma endregion

#pragma region 裏面回転
	// ボスの裏面回転
	BossBackRotate(Easing::Quint::easeOut(basicSpd_, boomerangRotaSpd_, rate));
#pragma endregion

#pragma region スプライトデータ更新
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
	sBossFront_->SetPosition(position_);
#pragma endregion
}

void Boss::Boomerang()
{
	if (isBoomerang_ == false) {
		backPos0_.x -= 30.0f;
		backPos1_.x -= 30.0f;

		if (backPos0_.x <= -300.0f) {
			isBoomerang_ = true;
			backPos0_.y = 240.0f;
			backPos1_.y = 240.0f;
		}
	}

	else {
		backPos0_.x += 30.0f;
		backPos1_.x += 30.0f;

		if (backPos0_.x >= 1500.0f) {
			isBoomerang_ = false;
			state_ = POST_BOOMERANG;
			actionStartTime_ = Util::GetTimrMSec();
			beforeBackPos0_ = backPos0_;
			beforeBackPos1_ = backPos1_;
		}
	}

#pragma region パーティクル
	for (size_t i = 0; i < 2; i++) {
		float rnd0X = Util::GetRandomFloat(0.0f, 360.0f);
		float rnd0Y = Util::GetRandomFloat(0.0f, 360.0f);
		Vector2 result0 = { cosf(rnd0X), sinf(rnd0Y) };
		result0.normalize();
		emitterBack0_->Add(60, { result0.x * 70.0f, result0.y * 70.0f }, { result0.x * 1.5f, result0.y * 1.5f }, { 0.0f, 0.0f }, 32.0f, 0.0f);
		emitterBack0_->SetPosition(backPos0_);

		float rnd1X = Util::GetRandomFloat(-360.0f, 0.0f);
		float rnd1Y = Util::GetRandomFloat(-360.0f, 0.0f);
		Vector2 result1 = { cosf(rnd1X), sinf(rnd1Y) };
		result1.normalize();
		emitterBack1_->Add(60, { result1.x * 70.0f, result1.y * 70.0f }, { result1.x * 1.5f, result1.y * 1.5f }, { 0.0f, 0.0f }, 32.0f, 0.0f);
		emitterBack1_->SetPosition(backPos1_);
	}
#pragma endregion

#pragma region 裏面回転
	// ボスの裏面回転
	BossBackRotate(boomerangRotaSpd_);
#pragma endregion

#pragma region スプライトデータ更新
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
#pragma endregion
}

void Boss::PostBoomerang()
{
#pragma region 遷移前座標から基本座標まで移動
	// 行動開始からの経過時間
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// 経過時間の割合で移動
	float rate = Util::Clamp(elapsedTime / time2PostBoomerang_, 1.0f, 0.0f);
	backPos0_.x = Easing::Quint::easeOut(beforeBackPos0_.x, basicPos_.x, rate);
	backPos0_.y = Easing::Quint::easeOut(beforeBackPos0_.y, basicPos_.y, rate);
	backPos1_.x = Easing::Quint::easeOut(beforeBackPos1_.x, basicPos_.x, rate);
	backPos1_.y = Easing::Quint::easeOut(beforeBackPos1_.y, basicPos_.y, rate);

	// 経過時間が指定時間以上ならStateをWAITにする
	if (elapsedTime >= time2PostBoomerang_) {
		state_ = WAIT;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region 裏面回転
	// ボスの裏面回転
	BossBackRotate(Easing::Quint::easeOut(boomerangRotaSpd_, basicSpd_, rate));
#pragma endregion

#pragma region スプライトデータ更新
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
#pragma endregion
}

void Boss::BossBackRotate(float rotate)
{
	rotateBossBack0_ += rotate;
	rotateBossBack1_ += rotate;
	rotateBossFront_ -= basicSpd_;
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack1_->SetRotation(rotateBossBack1_);
	sBossFront_->SetRotation(rotateBossFront_);
}

void Boss::DebugImGui()
{
	//ImGui::Begin("Boss");
	//ImGui::Text("State = %s", stateText_[state_].c_str());
	//ImGui::Text("Position = { %f, %f }", position_.x, position_.y);
	//ImGui::Text("BackPos0 = { %f, %f }", backPos0_.x, backPos0_.y);
	//ImGui::Text("BackPos1 = { %f, %f }", backPos1_.x, backPos1_.y);
	//if (ImGui::Button("Start Sumoon") && state_ == WAIT) {
	//	state_ = PRE_SUMMON;
	//	actionStartTime_ = Util::GetTimrMSec();
	//}

	//if (ImGui::Button("Start MoveShot") && state_ == WAIT) {
	//	state_ = PRE_MOVE_SHOT;
	//	actionStartTime_ = Util::GetTimrMSec();
	//}

	//if (ImGui::Button("Start Boomerang") && state_ == WAIT) {
	//	state_ = PRE_BOOMERANG;
	//	actionStartTime_ = Util::GetTimrMSec();
	//}
	//ImGui::End();

	if (key_->TriggerKey(DIK_8) && state_ == WAIT) {
		state_ = PRE_SUMMON;
		actionStartTime_ = Util::GetTimrMSec();
	}

	if (key_->TriggerKey(DIK_9) && state_ == WAIT) {
		state_ = PRE_MOVE_SHOT;
		actionStartTime_ = Util::GetTimrMSec();
	}

	if (key_->TriggerKey(DIK_0) && state_ == WAIT) {
		state_ = PRE_BOOMERANG;
		actionStartTime_ = Util::GetTimrMSec();
	}
}