#include "PlayerBullet.h"
#include"WinAPI.h"

//�ÓI�����o�̎���
Vector2 PlayerBullet::bulletSize = { 0,0 };
uint16_t PlayerBullet::bulletTex = 0;

void PlayerBullet::Initialize(const Vector2& pos, const Vector2& velocity)
{
	sprite = std::make_unique<Sprite>();
}

void PlayerBullet::Update()
{
	//��ʊO�ɏo���玀�S
	Vector2 windowMin = { 0,0 };
	Vector2 windowMax = { (float)WinAPI::GetInstance()->GetWidth(), (float)WinAPI::GetInstance()->GetWidth() };
	pos = sprite->GetPosition();
	if (pos.x < windowMin.x || pos.x > windowMax.x || pos.y < windowMin.y || pos.y > windowMax.y) {
		isAlive = false;
	}
}

void PlayerBullet::Draw()
{
	sprite->Draw(bulletTex);
}
