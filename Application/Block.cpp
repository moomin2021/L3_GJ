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
	// �o�^����
	sColMgr2D_->RemoveCollider(collider_.get());
}

void Block::Initialize()
{
#pragma region �X�v���C�g
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetSize({ 32.0f, 32.0f });
	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region �R���C�_�[
	collider_ = std::make_unique<BoxCollider>();
	collider_->SetAttribute(COL_BLOCK);
	collider_->SetSprite(sprite_.get());
	sColMgr2D_->AddCollider(collider_.get());
#pragma endregion
}

void Block::Update()
{
	// �s�[�X�ɏ������Ă��Ȃ�������v���C���[��ō��W���C��
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
	// �Փ˂��Ă����珊�����Ă���s�[�X�̏Փ˃t���O��[ON]�ɂ���
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
	// �����ύX
	collider_->SetAttribute(COL_PLAYER);
	Vector2 player2Block = sprite_->GetPosition() - sPlayer_->GetPosition();
	offset_.x = ceilf(player2Block.x / 32.0f);
	offset_.y = floorf(player2Block.y / 32.0f);

	isPiece_ = false;
}