#include "Player.h"
#include<imgui_impl_dx12.h>
#include"Easing.h"
#include"Util.h"
#include"CollisionAttribute.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize()
{
#pragma region インスタンス
	pad_ = Pad::GetInstance();
	colMgr2D_ = CollisionManager2D::GetInstance();
#pragma endregion

#pragma region スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetPosition({0.0f, 0.0f});
	sprite_->SetSize({ 32.0f, 32.0f });
	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region 画像ハンドル
	handle_ = LoadTexture("Resources/player.png");
#pragma endregion

#pragma region コライダー
	collider_ = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, Vector2{ 16.0f, 16.0f });
	collider_->SetAttribute(COL_PLAYER);
	collider_->SetSprite(sprite_.get());
	colMgr2D_->AddCollider(collider_.get());
#pragma endregion
}

void Player::Update()
{
	// 移動処理
	Move();

	// 回転処理
	Rotate();
}

void Player::Draw()
{
	sprite_->Draw(handle_);
}

void Player::OnCollision()
{
}

void Player::MatUpdate()
{
	sprite_->MatUpdate();
}

void Player::Move()
{
	// パッド入力を取得
	Vector2 input = pad_->GetLStick();

	// ウィンドウ座標を合わせて反転
	input.y = -input.y;

	// 座標を更新
	position_ += input * baseSpd_;

	// スプライトに座標を設定
	sprite_->SetPosition(position_);
}

void Player::Rotate()
{
	// 回転してからの経過時間[s]
	float elapsedTime = (Util::GetTimrMSec() - startRotateTime_) / 1000.0f;

	// 経過時間の割合が0.0f ~ 1.0fの間になるように
	float rate = Util::Clamp(elapsedTime / rotateTime_, 1.0f, 0.0f);

	// 経過時間の割合が1.0f以下なら
	if (rate <= 1.0f) {
		// 回転角を更新
		rotate_ = Easing::Circ::easeOut(beforeRotate_, afterRotate_, rate);

		// スプライトの回転角を設定
		sprite_->SetRotation(rotate_);
	}

	// 経過時間が回転時間異常なら回転入力を許可する
	if (elapsedTime >= rotateTime_) {
		// [LB]なら左回転
		if (pad_->GetTriggerButton(PAD_LB)) {
			// 回転開始時間を保存
			startRotateTime_ = Util::GetTimrMSec();

			// 回転前と回転後の回転角を保存
			beforeRotate_ = sprite_->GetRotation();
			afterRotate_ = beforeRotate_ - 90.0f;
		}

		// [RB]なら右回転
		else if (pad_->GetTriggerButton(PAD_RB)) {
			// 回転開始時間を保存
			startRotateTime_ = Util::GetTimrMSec();

			// 回転前と回転後の回転角を保存
			beforeRotate_ = sprite_->GetRotation();
			afterRotate_ = beforeRotate_ + 90.0f;
		}
	}
}