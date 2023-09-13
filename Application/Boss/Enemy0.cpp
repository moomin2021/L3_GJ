#include "Enemy0.h"
#include "CollisionAttribute.h"

uint16_t Enemy0::hBack_ = 0;
uint16_t Enemy0::hFront_ = 0;
CollisionManager2D* Enemy0::sColMgr2D_ = nullptr;

Enemy0::Enemy0() {}

Enemy0::~Enemy0() {
	sColMgr2D_->RemoveCollider(collider_.get());
}

void Enemy0::Initialize(Vector2 pos)
{
	// ���W�ݒ�
	position_ = pos;

#pragma region �X�v���C�g
	// ����
	sBack_ = std::make_unique<Sprite>();
	sBack_->SetPosition(position_);
	sBack_->SetSize({ 32.0f, 32.0f });
	sBack_->SetAnchorPoint({ 0.5f, 0.5f });

	// �\��
	sFront_ = std::make_unique<Sprite>();
	sFront_->SetPosition(position_);
	sFront_->SetSize({ 32.0f, 32.0f });
	sFront_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region �R���C�_�[
	collider_ = std::make_unique<CircleCollider>();
	collider_->SetAttribute(COL_ENEMY);
	collider_->SetSprite(sFront_.get());
	sColMgr2D_->AddCollider(collider_.get());
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

#pragma region ��ʊO�ɍs�����琶���t���O��[OFF]�ɂ���
	if (position_.x <= -100.0f && isAlive_) {
		isAlive_ = false;
	}
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
	if (collider_->GetIsHit()) {
		isAlive_ = false;
	}
}

void Enemy0::MatUpdate()
{
	sBack_->MatUpdate();
	sFront_->MatUpdate();
}