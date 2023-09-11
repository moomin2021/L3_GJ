#include "PlayerBullet.h"
#include"WinAPI.h"
#include<imgui_impl_dx12.h>

//静的メンバの実態
Vector2 PlayerBullet::bulletSize = { 0,0 };
uint16_t PlayerBullet::bulletTex = 0;

void PlayerBullet::SetBulletTex(uint16_t tex)
{
	bulletTex = tex;
}

void PlayerBullet::Initialize(const Vector2& pos, const Vector2& velocity)
{
	sprite = std::make_unique<Sprite>();
	this->pos = pos;
	sprite->SetPosition(pos);
	sprite->SetAnchorPoint({ 0.5f,0.5f });
	sprite->MatUpdate();
	sprite->SetSize({16.0f, 12.0f});
	this->vel = velocity;
}

void PlayerBullet::Update()
{
	//画面外に出たら死亡
	Vector2 windowMin = { 0,0 };
	Vector2 windowMax = { (float)WinAPI::GetInstance()->GetWidth(), (float)WinAPI::GetInstance()->GetWidth() };
	pos = sprite->GetPosition();
	pos += vel;

	//ImGui::Text("vel %f,%f", vel.x, vel.y);
	//ImGui::Text("pos %f,%f", pos.x, pos.y);

	sprite->SetPosition(pos);
	sprite->MatUpdate();
	if (pos.x < windowMin.x || pos.x > windowMax.x || pos.y < windowMin.y || pos.y > windowMax.y) {
		isAlive = false;
	}
}

void PlayerBullet::Draw()
{
	sprite->Draw(bulletTex);
}
