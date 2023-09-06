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

	colManager = CollisionManager2D::GetInstance();

	//�R���C�_�[�̃Z�b�g
	collider = std::make_unique<CircleCollider>(Vector2{ 0,0 },Block::GetBlockSize().x);
	//��������
	//collider->SetAttribute();
	collider->SetSprite(sprite.get());
	//�}�l�[�W���ɓo�^
	colManager->AddCollider(collider.get());
}

void Player::Update()
{

	//�ړ�
	Move();

	//��]
	Rotate();

	//�u���b�N�𑝂₷�f�o�b�O�֐�
	AddBlock();

	sprite->MatUpdate();
	UpdateBlocks();

}

void Player::Draw()
{
	//���@�`��
	sprite->Draw(texIndex);

	//�u���b�N�����̕`��
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->Draw();
	}

}

void Player::Move()
{
	//�p�b�h���͂ňړ�
	Vector2 spd;
	spd = pad->GetLStick() * baseSpd;
	spd.y = -spd.y;

	position = sprite->GetPosition();

	position += spd;

	sprite->SetPosition(position);
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

	ImGui::SliderInt("offsetX", &debugBlockOffsetX, -5, 5);
	ImGui::SliderInt("offsetY", &debugBlockOffsetY, -5, 5);

	if (ImGui::Button("add")) {
		//�ݒ肳��Ă���u���b�N�̃I�t�Z�b�g���g���ău���b�N�����A���@�ƕR����
		std::unique_ptr<Block> newBlock = std::make_unique<Block>();
		ParentData* parent = new ParentData();
		parent->parentPos = &position;
		parent->tileOffset = { (float)debugBlockOffsetX, (float)debugBlockOffsetY };
		parent->parentRot = &rotation;
		newBlock->Initialize(BlockData::None, parent);

		//���@�̃u���b�N�z��Ɋi�[
		blocks.push_back(std::move(newBlock));
	}

	ImGui::End();

}

void Player::UpdateBlocks()
{
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->Update();
	}
}
