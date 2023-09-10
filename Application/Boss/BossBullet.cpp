#include "BossBullet.h"

uint16_t BossBullet::sHandle_ = 0;
float BossBullet::sSpeed_ = 0;

BossBullet::BossBullet() {}

BossBullet::~BossBullet() {}

void BossBullet::Initialize(Vector2 pos)
{
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetSize({ 16.0f, 12.0f });
	position_ = pos;
}

void BossBullet::Update()
{
#pragma region �ړ�����
	position_.x -= sSpeed_;
	sprite_->SetPosition(position_);
#pragma endregion

#pragma region ��ʊO�ɍs�����琶���t���O��[OFF]�ɂ���
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

}

void BossBullet::MatUpdate()
{
	sprite_->MatUpdate();
}