#include "Player.h"
#include<imgui_impl_dx12.h>
#include"Easing.h"
#include"Util.h"

void Player::Initialize(uint16_t playerTexture, const Vector2& pos)
{
	texIndex = playerTexture;

	sprite = std::make_unique<Sprite>();

	//�X�v���C�g�̃T�C�Y���u���b�N�̃T�C�Y�ƍ��킹��
	sprite->SetSize(Block::GetBlockSize());
	sprite->SetPosition(pos);
	sprite->SetAnchorPoint({ 0.5f,0.5f });

	rotation = 0;

	pad = Pad::GetInstance();
}

void Player::Update()
{

	//�ړ�
	Move();

	//��]
	Rotate();

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
	


	float timerate = rotEaseTime / easeTimeMax;
	//timeRate��1�ȉ��Ȃ���
	if (timerate < 1.0f) {
		rotation = Easing::Circ::easeOut(beforeRot, afterRot, timerate);
		//��]�p���ʓx�@�ɕϊ�
		sprite->SetRotation(rotation);
		rotEaseTime++;
	}
	else {
		//�{�^���̃g���K�[�ŉ�]�����m
		//LB�L�[�ō���]�ARB�L�[�ŉE��]
		if (pad->GetTriggerButton(PAD_LB)) {
			beforeRot = sprite->GetRotation();
			afterRot = beforeRot - 90.0f;
			rotEaseTime = 0;
		}
		else if (pad->GetTriggerButton(PAD_RB)) {
			beforeRot = sprite->GetRotation();
			afterRot = beforeRot + 90.0f;
			rotEaseTime = 0;
		}

	}
	//�p�x��0~360�ɂȂ�悤�ɒ���

}

void Player::AddBlock()
{
	ImGui::Begin("add block");



	ImGui::End();

}
