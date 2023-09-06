#include "Player.h"
#include<imgui_impl_dx12.h>

void Player::Initialize(uint16_t playerTexture)
{
	texIndex = playerTexture;

	sprite = std::make_unique<Sprite>();

	//�X�v���C�g�̃T�C�Y���u���b�N�̃T�C�Y�ƍ��킹��
	sprite->SetSize(Block::GetBlockSize());

	rotation = 0;

	pad = Pad::GetInstance();
}

void Player::Update()
{

	//�ړ�
	Move();

	sprite->MatUpdate();

}

void Player::Draw()
{
	//���@�`��
	sprite->Draw();

	//�u���b�N�����̕`��
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i].Draw();
	}

}

void Player::Move()
{
	//�p�b�h���͂ňړ�
	Vector2 spd;
	spd = pad->GetLStick() * baseSpd;
	spd.y = -spd.y;

	Vector2 pos = sprite->GetPosition();

	pos += spd;

	sprite->SetPosition(pos);
}

void Player::Rotate()
{



	//�p�x��0~360�ɂȂ�悤�ɒ���

}

void Player::AddBlock()
{
	ImGui::Begin("add block");



	ImGui::End();

}
