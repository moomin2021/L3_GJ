#include "Player.h"
#include<imgui_impl_dx12.h>
#include"Easing.h"
#include"Util.h"
#include"CollisionAttribute.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize()
{
#pragma region �C���X�^���X
	pad_ = Pad::GetInstance();
	colMgr2D_ = CollisionManager2D::GetInstance();
#pragma endregion

#pragma region �X�v���C�g
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetPosition({0.0f, 0.0f});
	sprite_->SetSize({ 32.0f, 32.0f });
	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region �摜�n���h��
	handle_ = LoadTexture("Resources/player.png");
#pragma endregion

#pragma region �R���C�_�[
	collider_ = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, Vector2{ 16.0f, 16.0f });
	collider_->SetAttribute(COL_PLAYER);
	collider_->SetSprite(sprite_.get());
	colMgr2D_->AddCollider(collider_.get());
#pragma endregion
}

void Player::Update()
{
	// �ړ�����
	Move();

	// ��]����
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
	// �p�b�h���͂��擾
	Vector2 input = pad_->GetLStick();

	// �E�B���h�E���W�����킹�Ĕ��]
	input.y = -input.y;

	// ���W���X�V
	position_ += input * baseSpd_;

	// �X�v���C�g�ɍ��W��ݒ�
	sprite_->SetPosition(position_);
}

void Player::Rotate()
{
	// ��]���Ă���̌o�ߎ���[s]
	float elapsedTime = (Util::GetTimrMSec() - startRotateTime_) / 1000.0f;

	// �o�ߎ��Ԃ̊�����0.0f ~ 1.0f�̊ԂɂȂ�悤��
	float rate = Util::Clamp(elapsedTime / rotateTime_, 1.0f, 0.0f);

	// �o�ߎ��Ԃ̊�����1.0f�ȉ��Ȃ�
	if (rate <= 1.0f) {
		// ��]�p���X�V
		rotate_ = Easing::Circ::easeOut(beforeRotate_, afterRotate_, rate);

		// �X�v���C�g�̉�]�p��ݒ�
		sprite_->SetRotation(rotate_);
	}

	// �o�ߎ��Ԃ���]���Ԉُ�Ȃ��]���͂�������
	if (elapsedTime >= rotateTime_) {
		// [LB]�Ȃ獶��]
		if (pad_->GetTriggerButton(PAD_LB)) {
			// ��]�J�n���Ԃ�ۑ�
			startRotateTime_ = Util::GetTimrMSec();

			// ��]�O�Ɖ�]��̉�]�p��ۑ�
			beforeRotate_ = sprite_->GetRotation();
			afterRotate_ = beforeRotate_ - 90.0f;
		}

		// [RB]�Ȃ�E��]
		else if (pad_->GetTriggerButton(PAD_RB)) {
			// ��]�J�n���Ԃ�ۑ�
			startRotateTime_ = Util::GetTimrMSec();

			// ��]�O�Ɖ�]��̉�]�p��ۑ�
			beforeRotate_ = sprite_->GetRotation();
			afterRotate_ = beforeRotate_ + 90.0f;
		}
	}
}