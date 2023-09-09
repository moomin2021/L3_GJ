#include "Boss.h"
#include "Texture.h"
#include "Util.h"
#include "Easing.h"

#include <imgui_impl_DX12.h>

Boss::Boss() {}

Boss::~Boss() {
}

void Boss::Initialize()
{
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

	// デバック用
	DebugImGui();
}

void Boss::Draw()
{
	sBossBack0_->Draw(hBossBack_);// ボス裏面0
	sBossBack1_->Draw(hBossBack_);// ボス裏面1
	sBossFront_->Draw(hBossFront_);// ボス表面

	// 弾
	for (auto& it : bullets_) {
		it->Draw();
	}
}

void Boss::OnCollision()
{
	// 弾
	for (auto& it : bullets_) {
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
}

void (Boss::* Boss::stateTable[]) () = {
	&Boss::Wait,		// 待機
	&Boss::MoveShot,	// 移動撃ち
	&Boss::PreSummon,	// 召喚前準備
	&Boss::Summon,		// 召喚
	&Boss::PostSummon,	// 召喚後処理
	&Boss::Boomerang,	// ブーメラン
};

void Boss::Wait()
{
	// ボスの裏面回転
	BossBackRotate(basicSpd_);
}

void Boss::MoveShot()
{
	// ボスの裏面回転
	BossBackRotate(basicSpd_);

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
}

void Boss::PreSummon()
{
#pragma region 遷移前座標から基本座標まで移動
	// 行動開始からの経過時間
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// 経過時間の割合で移動
	float rate = Util::Clamp(elapsedTime / preSummonTime_, 1.0f, 0.0f);
	//position_.x = Easing::Quint::easeOut(beforePos_.x, basicPos_.x, rate);
	//position_.y = Easing::Quint::easeOut(beforePos_.y, basicPos_.y, rate);
	//backPos0_ = position_;
	//backPos1_ = position_;

	// 経過時間が指定時間以上ならStateをSummonにする
	if (elapsedTime >= preSummonTime_) {
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

#pragma region sin関数で移動処理
	sinMove_ += sinSpd_;
	backPos0_.y = (sinf(sinMove_) * 300.0f) + 540.0f;;
	backPos1_.y = (-sinf(sinMove_) * 300.0f) + 540.0f;;
#pragma endregion

#pragma region 召喚時間管理
	// 召喚してから指定の時間が経っていたら召喚を終了
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	if (elapsedTime >= summonTime_) {
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
	float rate = Util::Clamp(elapsedTime / postSummonTime_, 1.0f, 0.0f);
	backPos0_.x = Easing::Quint::easeOut(beforeBackPos0_.x, basicPos_.x, rate);
	backPos0_.y = Easing::Quint::easeOut(beforeBackPos0_.y, basicPos_.y, rate);
	backPos1_.x = Easing::Quint::easeOut(beforeBackPos1_.x, basicPos_.x, rate);
	backPos1_.y = Easing::Quint::easeOut(beforeBackPos1_.y, basicPos_.y, rate);

	// 経過時間が指定時間以上ならStateをSummonにする
	if (elapsedTime >= postSummonTime_) {
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

void Boss::Boomerang()
{

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
	ImGui::Begin("Boss");
	ImGui::Text("State = %s", stateText_[state_].c_str());
	ImGui::Text("Position = { %f, %f }", position_.x, position_.y);
	ImGui::Text("BackPos0 = { %f, %f }", backPos0_.x, backPos0_.y);
	ImGui::Text("BackPos1 = { %f, %f }", backPos1_.x, backPos1_.y);
	if (ImGui::Button("Start Sumoon")) {
		state_ = PRE_SUMMON;
		actionStartTime_ = Util::GetTimrMSec();
	}
	ImGui::End();
}