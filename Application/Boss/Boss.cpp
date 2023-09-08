#include "Boss.h"
#include "Texture.h"
#include "Util.h"

#include <imgui_impl_DX12.h>

Boss::Boss() {}

Boss::~Boss() {
}

void Boss::Initialize()
{
#pragma region スプライト
	// ボス裏面0
	sBossBack0_ = std::make_unique<Sprite>();
	sBossBack0_->SetPosition(position_);
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack0_->SetSize({ 160.0f, 128.0f });
	sBossBack0_->SetAnchorPoint({ 0.5f, 0.5f });

	// ボス裏面1
	sBossBack1_ = std::make_unique<Sprite>();
	sBossBack1_->SetPosition(position_);
	sBossBack1_->SetRotation(rotateBossBack1_);
	sBossBack1_->SetSize({ 160.0f, 128.0f });
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

#pragma region 弾
	// 画像ハンドルを設定
	BossBullet::SetHandle(LoadTexture("Resources/bullet_enemy.png"));

	// 速度を設定
	BossBullet::SetSpeed(10.0f);
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

	ImGui::Begin("Boss");
	ImGui::Text("Position = { %f, %f }", position_.x, position_.y);
	ImGui::Text("BulletNum = %d", bullets_.size());
	ImGui::End();
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
	&Boss::Summon,		// 召喚
	&Boss::Boomerang,	// ブーメラン
};

void Boss::Wait()
{
	// ボスの裏面を回転
	rotateBossBack0_ += rotateSpd_;
	rotateBossBack1_ += rotateSpd_;
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack1_->SetRotation(rotateBossBack1_);
}

void Boss::MoveShot()
{
	// ボスの裏面を回転
	rotateBossBack0_ += rotateSpd_;
	rotateBossBack1_ += rotateSpd_;
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack1_->SetRotation(rotateBossBack1_);

#pragma region 移動処理
	static float sin = 0.0f;
	sin += moveSpd_;
	position_.y = (sinf(sin) * 300.0f) + 540.0f;
	sBossBack0_->SetPosition(position_);
	sBossBack1_->SetPosition(position_);
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

void Boss::Summon()
{

}

void Boss::Boomerang()
{

}