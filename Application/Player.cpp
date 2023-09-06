#include "Player.h"
#include<imgui_impl_dx12.h>

void Player::Initialize(uint16_t playerTexture)
{
	texIndex = playerTexture;

	sprite = std::make_unique<Sprite>();

	//スプライトのサイズをブロックのサイズと合わせる
	sprite->SetSize(Block::GetBlockSize());

	rotation = 0;

	pad = Pad::GetInstance();
}

void Player::Update()
{

	//移動
	Move();

	sprite->MatUpdate();

}

void Player::Draw()
{
	//自機描画
	sprite->Draw();

	//ブロックたちの描画
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i].Draw();
	}

}

void Player::Move()
{
	//パッド入力で移動
	Vector2 spd;
	spd = pad->GetLStick() * baseSpd;
	spd.y = -spd.y;

	Vector2 pos = sprite->GetPosition();

	pos += spd;

	sprite->SetPosition(pos);
}

void Player::Rotate()
{



	//角度が0~360になるように調整

}

void Player::AddBlock()
{
	ImGui::Begin("add block");



	ImGui::End();

}
