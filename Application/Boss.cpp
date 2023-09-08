#include "Boss.h"
#include "Texture.h"

Boss::Boss() {}

Boss::~Boss() {}

void Boss::Initialize()
{
#pragma region スプライト
	// ボス裏面0
	sBossBack0_ = std::make_unique<Sprite>();
	sBossBack0_->SetPosition(position_);
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack0_->SetSize({ 128.0f, 128.0f });
	sBossBack0_->SetAnchorPoint({ 0.5f, 0.5f });

	// ボス裏面1
	sBossBack1_ = std::make_unique<Sprite>();
	sBossBack1_->SetPosition(position_);
	sBossBack1_->SetRotation(rotateBossBack1_);
	sBossBack1_->SetSize({ 128.0f, 128.0f });
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
}

void Boss::Update()
{
	// 状態別更新処理
	(this->*stateTable[state_])();
}

void Boss::Draw()
{
	sBossBack0_->Draw(hBossBack_);// ボス裏面0
	sBossBack1_->Draw(hBossBack_);// ボス裏面1
	sBossFront_->Draw(hBossFront_);// ボス表面
}

void Boss::OnCollision()
{
}

void Boss::MatUpdate()
{
	sBossBack0_->MatUpdate();// ボス裏面0
	sBossBack1_->MatUpdate();// ボス裏面1
	sBossFront_->MatUpdate();// ボス表面
}

void (Boss::* Boss::stateTable[]) () = {
	&Boss::Wait,		// 待機
	&Boss::MoveShot,	// 移動撃ち
	&Boss::Summon,		// 召喚
	&Boss::Boomerang,	// ブーメラン
};

void Boss::Wait()
{

}

void Boss::MoveShot()
{

}

void Boss::Summon()
{

}

void Boss::Boomerang()
{

}