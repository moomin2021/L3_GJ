#include "Block.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include<imgui_impl_dx12.h>
#include"Piece.h"
#include "Player.h"
#include "Piece.h"

CollisionManager2D* Block::sColMgr2D_ = nullptr;
Player* Block::sPlayer_ = nullptr;

Block::Block() {}

Block::~Block() {
	// 登録解除
	sColMgr2D_->RemoveCollider(collider_.get());
}

void Block::Initialize()
{
#pragma region スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetSize({ 32.0f, 32.0f });
	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region コライダー
	collider_ = std::make_unique<BoxCollider>();
	collider_->SetAttribute(COL_BLOCK);
	collider_->SetSprite(sprite_.get());
	sColMgr2D_->AddCollider(collider_.get());
#pragma endregion
}

void Block::Update()
{
	// ピースに所属していなかったらプレイヤー基準で座標を修正
	if (isPiece_ == false) {
		Vector2 newPos = sPlayer_->GetPosition() + offset_ * 32.0f;
		sprite_->SetPosition(newPos);
	}
}

void Block::Draw()
{
	sprite_->Draw(handle_);
}

void Block::OnCollision()
{
	// 衝突していたら所属しているピースの衝突フラグを[ON]にする
	if (collider_->GetIsHit()) {
		if (collider_->GetHitCollider()->GetAttribute() == COL_PLAYER) {
			piece_->SetIsCol(true);
		}
	}
}

void Block::MatUpdate()
{
	sprite_->MatUpdate();
}

void Block::SetAffChangePlayer()
{
	// 属性変更
	collider_->SetAttribute(COL_PLAYER);
	Vector2 player2Block = sprite_->GetPosition() - sPlayer_->GetPosition();
	offset_.x = ceilf(player2Block.x / 32.0f);
	offset_.y = floorf(player2Block.y / 32.0f);

	isPiece_ = false;
}