#include "Enemy0.h"

uint16_t Enemy0::hBack_ = 0;
uint16_t Enemy0::hFront_ = 0;

Enemy0::Enemy0() {}

Enemy0::~Enemy0() {}

void Enemy0::Initialize(Vector2 pos)
{
	// ���W�ݒ�
	position_ = pos;

#pragma region �X�v���C�g
	// ����
	sBack_ = std::make_unique<Sprite>();
	sBack_->SetPosition(position_);
	sBack_->SetAnchorPoint({ 0.5f, 0.5f });

	// �\��
	sFront_ = std::make_unique<Sprite>();
	sFront_->SetPosition(position_);
	sFront_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion
}

void Enemy0::Update()
{
#pragma region �ړ�����
	position_.x -= speed_;
#pragma endregion

#pragma region ��]����
	rotate_ += rotateSpd_;
#pragma endregion

#pragma region �X�v���C�g�f�[�^�X�V
	// ����
	sBack_->SetPosition(position_);
	sBack_->SetRotation(rotate_);

	// �\��
	sFront_->SetPosition(position_);
#pragma endregion
}

void Enemy0::Draw()
{
	sBack_->Draw(hBack_);
	sFront_->Draw(hFront_);
}

void Enemy0::OnCollision()
{

}

void Enemy0::MatUpdate()
{
	sBack_->MatUpdate();
	sFront_->MatUpdate();
}