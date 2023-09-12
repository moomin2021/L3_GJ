#include "PlayerBullet.h"
#include "WinAPI.h"
#include "CollisionAttribute.h"

#include<imgui_impl_dx12.h>

//�ÓI�����o�̎���
Vector2 PlayerBullet::sBulletSize_ = { 0.0f,0.0f };
uint16_t PlayerBullet::sHandle_ = 0;
Boss* PlayerBullet::sBoss_ = nullptr;
CollisionManager2D* PlayerBullet::sColMgr2D_ = nullptr;

PlayerBullet::~PlayerBullet()
{
	sColMgr2D_->RemoveCollider(collider_.get());
}

void PlayerBullet::Initialize(const Vector2& pos, const Vector2& velocity)
{
	// �����̒l��ݒ�
	position_ = pos;// ���W
	velocity_ = velocity;// �ړ���

	// �X�v���C�g
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetPosition(position_);
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprite_->SetSize({16.0f, 12.0f});

	// �R���C�_�[
	collider_ = std::make_unique<BoxCollider>(Vector2{ 0.0f, 0.0f }, Vector2{ 8.0f, 6.0f });
	collider_->SetAttribute(COL_PLAYER_BULLET);
	collider_->SetSprite(sprite_.get());
	sColMgr2D_->AddCollider(collider_.get());
}

void PlayerBullet::Update()
{
	//��ʊO�ɏo���玀�S
	Vector2 windowMin = { 0,0 };
	Vector2 windowMax = { (float)WinAPI::GetInstance()->GetWidth(), (float)WinAPI::GetInstance()->GetWidth() };
	position_ = sprite_->GetPosition();
	position_ += velocity_;

	//ImGui::Text("vel %f,%f", vel.x, vel.y);
	//ImGui::Text("pos %f,%f", pos.x, pos.y);

	sprite_->SetPosition(position_);
	if (position_.x < windowMin.x || position_.x > windowMax.x || position_.y < windowMin.y || position_.y > windowMax.y) {
		isAlive_ = false;
	}
}

void PlayerBullet::Draw()
{
	sprite_->Draw(sHandle_);
}

void PlayerBullet::OnCollision(uint16_t damage)
{
	if (collider_->GetIsHit()) {
		if (collider_->GetHitCollider()->GetAttribute() == COL_BOSS) {
			//������value�ɒe�̈З͂�����
			sBoss_->SubHP(damage);
			isAlive_ = false;
		}
	}
}

void PlayerBullet::MatUpdate()
{
	sprite_->MatUpdate();
}