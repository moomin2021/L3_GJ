#include "BossBullet.h"
#include "CollisionAttribute.h"

uint16_t BossBullet::sHandle_ = 0;
float BossBullet::sSpeed_ = 0;
CollisionManager2D* BossBullet::sColMgr2D_ = nullptr;

BossBullet::BossBullet() {}

BossBullet::~BossBullet()
{
	sColMgr2D_->RemoveCollider(collider_.get());
}

void BossBullet::Initialize(Vector2 pos)
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetSize({ 24.0f, 18.0f });
	position_ = pos;

#pragma region コライダー
	collider_ = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, Vector2{ 8.0f, 6.0f });
	collider_->SetAttribute(COL_BOSS_BULLET);
	collider_->SetSprite(sprite_.get());
	sColMgr2D_->AddCollider(collider_.get());
#pragma endregion
}

void BossBullet::Update()
{
#pragma region 移動処理
	position_.x -= sSpeed_;
	sprite_->SetPosition(position_);
#pragma endregion

#pragma region 画面外に行ったら生存フラグを[OFF]にする
	if (position_.x <= -100.0f && isAlive_) {
		isAlive_ = false;
	}
#pragma endregion
}

void BossBullet::Draw()
{
	sprite_->Draw(sHandle_);
}

void BossBullet::OnCollision()
{
	if (collider_->GetIsHit()) {
		if (collider_->GetHitCollider()->GetAttribute() == COL_PLAYER) {
			isAlive_ = false;
		}
	}
}

void BossBullet::MatUpdate()
{
	sprite_->MatUpdate();
}