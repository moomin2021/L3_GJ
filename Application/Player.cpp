#include "Player.h"
#include<imgui_impl_dx12.h>
#include"Easing.h"
#include"Util.h"
#include"CollisionAttribute.h"

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
	key = Key::GetInstance();

	colManager = CollisionManager2D::GetInstance();

	//�R���C�_�[�̃Z�b�g
	Vector2 size = Block::GetBlockSize();
	//����͎��ۂ̑傫����90����
	size *= 0.9f; 

	collider = std::make_unique<BoxCollider>(Vector2{ 0,0 }, Vector2(size.x / 2.0f, size.y / 2.0f));
	//��������
	collider->SetAttribute(COL_PLAYER);
	collider->SetSprite(sprite.get());
	//�}�l�[�W���ɓo�^
	colManager->AddCollider(collider.get());

	//�̗͊֌W�̏�����
	health = healthMax;

}

void Player::Update()
{

	//�ړ�
	Move();

	//��]
	Rotate();

	//�U��
	Attack();

	//�e�̍X�V
	BulletsUpdate();

	//�u���b�N�𑝂₷�f�o�b�O�֐�
	AddBlock();

	//�u���b�N���Z�b�g
	BlockReset();

	sprite->MatUpdate();
	UpdateBlocks();

	ImGui::Text("pos %f,%f", position.x, position.y);
	ImGui::Text("health %d", health);
	ImGui::Text("Lv.%d : EXP %d", level, currentEXP);

}

void Player::Draw()
{
	//���@�`��
	sprite->Draw(texIndex);

	//�u���b�N�����̕`��
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->Draw();
	}

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}

}

void Player::OnCollision()
{
	//�����ɓ���������
	if (collider->GetIsHit()) {

		if (collider->GetHitCollider()->GetAttribute() == COL_BLOCK) {
			Vector2 hitBlockOffset{ 0,0 };

			//�u���b�N�ɓ�������
			//���@����Ώۂւ̃x�N�g���쐬
			Vector2 vecBtoP = collider->GetHitCollider()->GetPosition() - collider->GetPosition();
			//x������y�����̐�Βl���r���A�c�ɂ������ɂ������߂�
			if (fabs(vecBtoP.x) > fabs(vecBtoP.y)) {
				//x�����̂ق����傫���ꍇ
				if (vecBtoP.x > 0) {
					//0�ȏ�Ȃ��ɂ���
					hitBlockOffset.x = 1.0f;
				}
				else {
					//0�ȉ��Ȃ牺
					hitBlockOffset.x = -1.0f;
				}
			}
			else {
				if (vecBtoP.y > 0) {
					//0�ȏ�Ȃ��ɂ���
					hitBlockOffset.y = 1.0f;
				}
				else {
					//0�ȉ��Ȃ牺
					hitBlockOffset.y = -1.0f;
				}
			}

			//�Փ˂����u���b�N���̂̂������ꏊ��ݒ�ł����̂ŁA�s�[�X�̃u���b�N���ׂĂɑ΂���

		}

	}
}

void Player::Move()
{
	//�p�b�h���͂ňړ�
	Vector2 spd;
	spd = pad->GetLStick() * baseSpd;

	if (key->PushKey(DIK_W) || key->PushKey(DIK_A) || key->PushKey(DIK_S) || key->PushKey(DIK_D)) {
		spd.x = (key->PushKey(DIK_D) - key->PushKey(DIK_A)) * baseSpd;
		spd.y = (key->PushKey(DIK_W) - key->PushKey(DIK_S)) * baseSpd;
	}


	spd.y = -spd.y;

	position = sprite->GetPosition();

	position += spd;

	sprite->SetPosition(position);
}

void Player::Rotate()
{



	float timerate = rotEaseTime / easeTimeMax;
	//timerate�̏㉺���ݒ�
	//timerate = Util::Clamp(timerate, 1.0f, 0.0f);

	ImGui::Text("rotEaseTime %f", rotEaseTime);
	ImGui::Text("easeTimeMax %f", easeTimeMax);
	ImGui::Text("timerate %f", timerate);
	ImGui::Text("rotate %f", rotation);



	//timeRate��1�ȉ��Ȃ���
	if (timerate <= 1.0f) {



		rotation = beforeRot + Easing::Circ::easeOut(0.0f, childRotation, timerate);


		//��]�p���ʓx�@�ɕϊ�
		sprite->SetRotation(rotation);
		rotEaseTime++;

		for (size_t i = 0; i < blocks.size(); i++) {
			ParentData* parent = blocks[i]->GetParent();
			parent->parentRot = Easing::Circ::easeOut(0.0f, childRotation, timerate);
		}
	}
	else {
		//�{�^���̃g���K�[�ŉ�]�����m
		//LB�L�[�ō���]�ARB�L�[�ŉE��]
		if (pad->GetTriggerButton(PAD_LB) || key->TriggerKey(DIK_J)) {
			beforeRot = sprite->GetRotation();
			afterRot = beforeRot - 90.0f;
			rotEaseTime = 0;
		}
		else if (pad->GetTriggerButton(PAD_RB) || key->TriggerKey(DIK_K)) {
			beforeRot = sprite->GetRotation();
			afterRot = beforeRot + 90.0f;
			rotEaseTime = 0;
		}

		////�q�̉�]�̑傫��
		childRotation = afterRot - beforeRot;
		//�q�̃I�t�Z�b�g�X�V(�q�̊֐����ň�x�����X�V���Ȃ��悤�ɂ���
		for (size_t i = 0; i < blocks.size(); i++) {
			blocks[i]->OffsetUpdate();
		}
	}

	//�p�x��0~360�ɂȂ�悤�ɒ���

}

void Player::Attack()
{
	//�ˌ��̃N�[���^�C�����炷
	if (shotCooltime > 0) {
		shotCooltime--;
	}
	else {
		//0�ȉ��Ȃ�N�[���^�C�����Z�b�g�AID��cannon�̃u���b�N����̂ݍU�����s��
		for (size_t i = 0; i < blocks.size(); i++) {
			if (blocks[i]->GetBlockData() == BlockData::Cannon) {
				Vector2 pos = blocks[i]->GetPosition();
				Vector2 vel(16.0f, 0.0f);
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

				newBullet->Initialize(pos, vel);

				bullets.push_back(std::move(newBullet));
			}
		}

		shotCooltime = shotCooltimeMax;
	}
}

void Player::AddBlock()
{

	ImGui::Begin("add block");

	ImGui::SliderInt("offsetX", &debugBlockOffsetX, -5, 5);
	ImGui::SliderInt("offsetY", &debugBlockOffsetY, -5, 5);

	if (ImGui::Button("add")) {
		//�ݒ肳��Ă���u���b�N�̃I�t�Z�b�g���g���ău���b�N�����A���@�ƕR����
		ParentData* parent = new ParentData();
		parent->parentPos = position;
		parent->tileOffset = { (float)debugBlockOffsetX, (float)debugBlockOffsetY };
		parent->parentRot = rotation;
		Block* newBlock = Block::CreateBlock(BlockData::None, parent);

		//���@�̃u���b�N�z��Ɋi�[
		blocks.push_back(newBlock);
	}

	ImGui::End();

}

void Player::BulletsUpdate()
{
	//����ł�e������
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});

	//�e�̍X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

}

void Player::AddBlock(Block* block)
{
	//�z��ɑ}��
	blocks.push_back(block);
}

void Player::UpdateBlocks()
{
	for (size_t i = 0; i < blocks.size(); i++) {
		//�e�̍��W�Ɖ�]�p���X�V��������
		ParentData* parent = blocks[i]->GetParent();
		parent->parentPos = position;
		//	parent->parentRot = &rotation;
		blocks[i]->SetParent(parent);
		blocks[i]->Update();
		ImGui::Text("blocks[%d]offset:%1.f,%1.f", i, blocks[i]->GetOffset().x, blocks[i]->GetOffset().y);
	}
}

void Player::UpdateOffset()
{
}

void Player::BlockReset()
{
	//A�{�^���g���K�[�Ń��Z�b�g
	if (pad->GetTriggerButton(BUTTON::PAD_A) || key->TriggerKey(DIK_SPACE)) {
		//�z��ɂ���u���b�N�̐���ۑ�
		int blockCount = (int)blocks.size();

		//TODO:�`������Ă���`�����m���ĉ��Z����o���l�ɔ{����������

		//�o���l�ɉ��Z
		currentEXP += blockCount;

		//�z����N���A
		blocks.clear();

	}

}
